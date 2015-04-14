namespace pcf {

class ply_reader_error : public std::runtime_error {
public:
	ply_reader_error(const std::string& str) : std::runtime_error(str) {}
};




template<typename Point>
void ply_reader::read(Point* buffer, std::size_t n) {
	if(current_element_ + n > number_of_vertices_)
		throw ply_reader_error("Attempted to read beyond bounds.");
	
	if(is_ascii()) read_ascii_(buffer, n);
	else read_binary_(buffer, n);
	
	current_element_ += n;
}


template<typename Point>
void ply_reader::read_ascii_(Point* buffer, std::size_t n) {
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
void ply_reader::read_binary_(Point* out, std::size_t n) {
	std::streamsize length = n * vertex_length_;
	std::unique_ptr<char[]> buffer(new char[length]);
	file_.read(buffer.get(), length);
	
	char* buffer_end = buffer.get() + length;
	for(char* in = buffer.get(); in != buffer_end; in += vertex_length_, ++out)
		read_binary_point_(*out, in);
}


template<typename T>
T ply_reader::read_binary_property_(const property& prop, char* data) const {
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

}