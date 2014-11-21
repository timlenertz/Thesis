#include "ply_importer.h"
#include <algorithm>

namespace pcf {

template<typename Point>
void ply_importer::read_ascii_(Point* buffer, std::size_t n) {
	std::string line;
	std::vector<const char*> properties(number_of_properties_, nullptr);
	
	while(n--) {
		read_line_(line);
				
		auto property = properties.begin();
		
		bool last_space = true;
		for(const char& c : line) {
			bool space = std::isspace(c);
			if(last_space == space) continue;
			else if(last_space) *(property++) = &c;
			last_space = space;
		}
		
		read_ascii_point_(*buffer, properties.data());
		++buffer;
	}
}


template<typename Point>
void ply_importer::read_binary_(Point* out, std::size_t n) {
	std::streamsize length = n * vertex_length_;
	std::unique_ptr<char[]> buffer(new char[length]);
	file_.read(buffer.get(), length);
	
	char* buffer_end = buffer.get() + length;
	for(char* in = buffer.get(); in != buffer_end; in += vertex_length_, ++out)
		read_binary_point_(*out, in);
}


template<typename T>
T ply_importer::read_binary_property_(const property& prop, char* data) const {
	std::size_t sz = property_type_size_(prop.type);
	data += prop.offset;
	if(! is_host_endian_binary_()) flip_endianness(data, sz);
	switch(prop.type) {
		case int8:   return static_cast<T>( *reinterpret_cast<const std::int8_t*>(data) );
		case uint8:  return static_cast<T>( *reinterpret_cast<const std::uint8_t*>(data) );
		case int16:  return static_cast<T>( *reinterpret_cast<const std::int16_t*>(data) );
		case uint16: return static_cast<T>( *reinterpret_cast<const std::uint16_t*>(data) );
		case int32:  return static_cast<T>( *reinterpret_cast<const std::int32_t*>(data) );
		case uint32: return static_cast<T>( *reinterpret_cast<const std::uint32_t*>(data) );
		default: break;
	}
	if(host_has_iec559_float) switch(prop.type) {
		case float32: return static_cast<T>( *reinterpret_cast<const float*>(data) );
		case float64: return static_cast<T>( *reinterpret_cast<const double*>(data) );
		default: break;
	}
	return T();
}



ply_importer::property_type ply_importer::identify_property_type_(const std::string& nm) {
	if(nm == "char" || nm == "int8") return int8;
	else if(nm == "uchar" || nm == "uint8") return uint8;
	else if(nm == "short" || nm == "int16") return int16;
	else if(nm == "ushort" || nm == "uint16") return uint16;
	else if(nm == "int" || nm == "int32") return int32;
	else if(nm == "uint" || nm == "uint32") return uint32;
	else if(nm == "float" || nm == "float32") return float32;
	else if(nm == "double" || nm == "float64") return float64;
	else if(nm == "list") return list;
	else throw ply_importer_error(std::string("Unknown property type ") + nm);
}


ply_importer::property* ply_importer::identify_property_(const std::string& nm_orig) {
	std::string nm = to_lower(nm_orig);
	if(nm == "x") return &x_;
	else if(nm == "y") return &y_;
	else if(nm == "z") return &z_;
	else if(nm == "nx") return &nx_;
	else if(nm == "ny") return &ny_;
	else if(nm == "nz") return &nz_;
	else if(nm == "r" || nm == "red") return &r_;
	else if(nm == "g" || nm == "green") return &g_;
	else if(nm == "b" || nm == "blue") return &b_;
	else return nullptr;
}


void ply_importer::read_header_() {
	std::string line;
		
	std::ifstream::pos_type data_start = 0; // File offset where data starts (aka after 'end_header').
	std::size_t number_of_elements_before_vertex_data = 0; // Number of data entries before start of vertex data. (for ascii, number of lines)
	std::ifstream::off_type vertex_data_start_offset = 0; // Number of bytes before start of vertex data. (for ascii, determined in postprocessing)
		
	std::ptrdiff_t vertex_property_index = 0; // Index of current vertex property.
	std::ptrdiff_t vertex_property_data_offset = 0; // Data offset of current vertex property for binary.


	// First line must be 'ply'.
	read_line_(line);
	if(line != "ply") throw ply_importer_error("Not PLY file (First line not 'ply')");
	
	// Second line must indicate format.
	read_line_(line);
	if(line == "format binary_little_endian 1.0") format_ = binary_little_endian;
	else if(line == "format binary_big_endian 1.0") format_ = binary_big_endian;
	else if(line == "format ascii 1.0") format_ = ascii;
	else throw ply_importer_error("Unknown PLY format: " + line);
	
	// Definitions of elements follow.
	// State before_vertex: Skip definitions until vertex element definition starts.
	//       within_vertex: Reading vertex property definitions.
	//       after_vertex: Skip other defitions after it, until end of header.
	enum { before_vertex_definition, within_vertex_definition, after_vertex_definition } state = before_vertex_definition;
	std::size_t number_of_elements = 0; // Number of elements of current type.
	do {
		read_line_(line);
		
		if(line.substr(0, 8) == "comment ") continue; // Skip comments.
		if(line.substr(0, 9) == "obj_info ") continue;
		
		if(line.substr(0, 8) == "element ") { // Read element definition start.
			auto pos = line.find(' ', 8);
			std::string element = line.substr(8, pos-8);
			number_of_elements = stoul(line.substr(pos + 1)); // Number of elements.
			if(element == "vertex") {
				// Vertex definitions start.
				if(state != before_vertex_definition) throw ply_importer_error("More than one vertex element definitions.");
				state = within_vertex_definition;
				number_of_vertices_ = number_of_elements; // Get number of vertices.
			} else if(state == within_vertex_definition) {
				// Another element definition after vertices.
				state = after_vertex_definition;
			} else if(state == before_vertex_definition) {
				// Element before vertex definition.
				// Count number of lines before vertex data start (for ascii).
				number_of_elements_before_vertex_data += number_of_elements;
			}
		
		} else if(line.substr(0, 9) == "property ") { // Reading property definition for current element.
			if(state == after_vertex_definition) continue; // After vertices: skip
			
			// Data type and property name
			auto pos = line.find(' ', 9);
			std::string name = line.substr(pos + 1);
			property_type type = identify_property_type_(line.substr(9, pos-9));
			if(type == list) throw ply_importer_error("List property type not supported.");			
			std::size_t type_size = property_type_size_(type);
			
			if(is_binary() && state == before_vertex_definition) {
				// If before vertex, count this entries of this property in total length of data before vertex data (only for binary).
				vertex_data_start_offset += number_of_elements * type_size;
				
			} else if(state == within_vertex_definition) {
				// When withnin vertex definition, capture XYZRGB data offsets.
				property* prop = identify_property_(name);
				if(prop) {
					// Store data offset (for binary) and index (for ascii) of property
					// If prop==null, unknown property
					prop->type = type;
					prop->offset = vertex_property_data_offset;
					prop->index = vertex_property_index;
				}
				// Increment data offset and index
				vertex_property_data_offset += type_size;
				++vertex_property_index;
			}
			
		} else if(line.substr(0, 10) == "end_header") { // End of header; data follows.
			if(state == before_vertex_definition) throw ply_importer_error("No vertex element definition.");
			data_start = file_.tellg(); // Start of data.

		} else if(! std::all_of(line.begin(), line.end(), isspace)) {
			// Invalid line, not all whitespace.
			throw ply_importer_error("Invalid line encountered: " + line);
		}
	} while(! data_start);
	
	if(!x_ || !y_ || !z_) throw ply_importer_error("X, Y, Z coordinate properties not defined.");
	has_rgb_ = (r_ && g_ && b_);
	
	vertex_length_ = vertex_property_data_offset;
	number_of_properties_ = vertex_property_index;
	
	if(is_binary()) {
		// Directly calculate vertex data start
		vertex_data_start_ = data_start + vertex_data_start_offset;
	} else {
		// Byte offset unknown, need to skip lines (of variable lengths) to it.
		skip_lines_(number_of_elements_before_vertex_data);
		vertex_data_start_ = file_.tellg();
	}
}


void ply_importer::rewind() {
	file_.seekg(vertex_data_start_);
	current_element_ = 0;
}


ply_importer::ply_importer(const char* filename, line_delimitor ld) :
file_(filename, std::ios_base::in | std::ios_base::binary),
line_delimitor_(ld != line_delimitor::unknown ? ld : detect_line_delimitor(file_)) {
	read_header_();	
	rewind();
}


std::size_t ply_importer::property_type_size_(property_type t) {
	switch(t) {
		case int8:  case uint8:  return 1;
		case int16: case uint16: return 2;
		case int32: case uint32: return 4;
		case float32: return 4;
		case float64: return 8;
		default: return 0;
	}
}


void ply_importer::read_ascii_point_(point_xyz& pt, const char* props[]) const {	
	pt = point_xyz(
		strtof(props[x_.index], nullptr),
		strtof(props[y_.index], nullptr),
		strtof(props[z_.index], nullptr)	
	);
}



void ply_importer::read_ascii_point_(point_full& pt, const char* props[]) const {
	if(has_rgb_) pt = point_full(
		strtof(props[x_.index], nullptr),
		strtof(props[y_.index], nullptr),
		strtof(props[z_.index], nullptr),
		strtol(props[r_.index], nullptr, 10),
		strtol(props[g_.index], nullptr, 10),
		strtol(props[b_.index], nullptr, 10)
	);
	else pt = point_xyz(
		strtof(props[x_.index], nullptr),
		strtof(props[y_.index], nullptr),
		strtof(props[z_.index], nullptr)	
	);
}


void ply_importer::read_binary_point_(point_xyz& pt, char* data) const {
	pt = point_xyz(
		read_binary_property_<float>(x_, data),
		read_binary_property_<float>(y_, data),
		read_binary_property_<float>(z_, data)
	);
}


void ply_importer::read_binary_point_(point_full& pt, char* data) const {
	if(has_rgb_) pt = point_full(
		read_binary_property_<float>(x_, data),
		read_binary_property_<float>(y_, data),
		read_binary_property_<float>(z_, data),
		read_binary_property_<std::uint8_t>(r_, data),
		read_binary_property_<std::uint8_t>(g_, data),
		read_binary_property_<std::uint8_t>(b_, data)
	);
	else pt = point_xyz(
		read_binary_property_<float>(x_, data),
		read_binary_property_<float>(y_, data),
		read_binary_property_<float>(z_, data)
	);
}


void ply_importer::read(point_xyz* buffer, std::size_t n) {
	if(current_element_ + n > number_of_vertices_)
		throw ply_importer_error("Attempted to read beyond bounds.");
	
	if(is_ascii()) read_ascii_(buffer, n);
	else read_binary_(buffer, n);
	
	current_element_ += n;
}


void ply_importer::read(point_full* buffer, std::size_t n) {
	if(current_element_ + n > number_of_vertices_)
		throw ply_importer_error("Attempted to read beyond bounds.");
	
	if(is_ascii()) read_ascii_(buffer, n);
	else read_binary_(buffer, n);
	
	current_element_ += n;
}


std::ptrdiff_t ply_importer::tell() const {
	return current_element_;
}


std::size_t ply_importer::size() const {
	return number_of_vertices_;
}


bool ply_importer::all_valid() const {
	return true;
}


}