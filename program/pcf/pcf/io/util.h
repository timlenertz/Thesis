#ifndef PCF_UTIL_H_
#define PCF_UTIL_H_

#include <iosfwd>
#include <limits>
#include <string>

namespace pcf {

enum class line_delimitor { unknown, LF, CR, CRLF };

const extern line_delimitor default_line_delimitor;
const extern bool host_has_iec559_float;
const extern bool host_is_little_endian;

line_delimitor detect_line_delimitor(std::istream&);

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