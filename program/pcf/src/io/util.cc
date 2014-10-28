#include "util.h"

#include <istream>
#include <ostream>
#include <stdexcept>
#include <type_traits>
#include <cctype>

namespace pcf {

static bool check_host_little_endian_() {
	unsigned int i = 1;
	char* c = reinterpret_cast<char*>(&i);
	return *c;
}


line_delimitor detect_line_delimitor(std::istream& str) {
	constexpr std::size_t max_counter = 512;
	std::size_t counter = 0;
	line_delimitor ld = line_delimitor::unknown;
	auto old_position = str.tellg();
	while(ld == line_delimitor::unknown) {
		if(++counter > max_counter) throw std::runtime_error("Could not detect file line ending");
		char c = str.get();
		if(c == '\n') ld = line_delimitor::LF;
		else if(c == '\r') {
			if(str.peek() == '\n') ld = line_delimitor::CRLF;
			else ld = line_delimitor::CR;
		}
	}
	str.seekg(old_position);
	return ld;
}

void read_line(std::istream& str, std::string& line, line_delimitor ld) {
	if(ld == line_delimitor::unknown) throw std::invalid_argument("Line delimitor not specified");
	
	if(ld == line_delimitor::CRLF) {
		std::getline(str, line, '\r');
		while(str.peek() != '\n') {
			std::string part_line;
			std::getline(str, part_line, '\r');
			line.append(1, '\r').append(part_line);
		}
		str.get();
	} else {
		std::getline(str, line, (ld == line_delimitor::LF ? '\n' : '\r'));
	}
}

void skip_line(std::istream& str, line_delimitor ld) {
	if(ld == line_delimitor::unknown) throw std::invalid_argument("Line delimitor not specified");
	constexpr auto max_count = std::numeric_limits<std::streamsize>::max();

	if(ld == line_delimitor::CRLF) {
		str.ignore(max_count, '\r');
		while(str.peek() != '\n') str.ignore(max_count, '\r');
		str.ignore();
	} else {
		str.ignore(max_count, (ld == line_delimitor::LF ? '\n' : '\r'));
	}
}

void write_line(std::ostream& str, const std::string& line, line_delimitor ld) {
	str << line;
	switch(ld) {
		case line_delimitor::LF: str << '\n'; break;
		case line_delimitor::CR: str << '\r'; break;
		case line_delimitor::CRLF: str << "\r\n"; break;
		default: throw std::invalid_argument("Line delimitor not specified");
	}
}


bool host_is_little_endian() {
	static bool little_endian = check_host_little_endian_();
	return little_endian;
}


void flip_endianness(char* data, std::size_t sz) {
	if(sz < 2) return;
	std::ptrdiff_t i = sz/2 - 1;
	std::ptrdiff_t o = sz - i - 1;
	while(i >= 0) std::swap(data[i--], data[o++]);
}


std::string to_lower(const std::string& s_orig) {
	std::string s(s_orig);
	for(char& c: s) c = std::tolower(c);
	return s;
}


std::string to_upper(const std::string& s_orig) {
	std::string s(s_orig);
	for(char& c: s) c = std::toupper(c);
	return s;
}


}