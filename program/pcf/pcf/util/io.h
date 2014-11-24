#ifndef PCF_UTIL_IO_H_
#define PCF_UTIL_IO_H_

#include <iosfwd>
#include <limits>
#include <string>

namespace pcf {

enum class line_delimitor { unknown, LF, CR, CRLF };

const extern line_delimitor default_line_delimitor;
const extern bool host_has_iec559_float;
const extern bool host_is_little_endian;

/**
Detects line delimitor used in given file.
Reads up to max_offset characters into file until line delimitor is found. Reads from current
stream position, and rewinds to that starting position afterwards. Throws exception if no
line ending detected.
*/
line_delimitor detect_line_delimitor(std::istream&, std::size_t max_offset = 512);

void read_line(std::istream&, std::string&, line_delimitor = default_line_delimitor);
void skip_line(std::istream&, line_delimitor = default_line_delimitor);
void write_line(std::ostream&, const std::string&, line_delimitor = default_line_delimitor);
void end_line(std::ostream&, line_delimitor = default_line_delimitor);

void flip_endianness(char* data, std::size_t sz);

template<typename T> void flip_endianness(T& t) {
	flip_endianness(reinterpret_cast<char*>(&t), sizeof(T));
}

std::string to_lower(const std::string&);
std::string to_upper(const std::string&);

}

#endif