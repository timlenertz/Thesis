#ifndef PCF_UTIL_H_
#define PCF_UTIL_H_

#include <iosfwd>

namespace pcf {

enum class line_delimitor { unknown, LF, CR, CRLF };

static constexpr line_delimitor default_line_delimitor = line_delimitor::LF;

line_delimitor detect_line_delimitor(std::istream&);

void read_line(std::istream&, std::string&, line_delimitor = default_line_delimitor);
void skip_line(std::istream&, line_delimitor = default_line_delimitor);
void write_line(std::ostream&, const std::string&, line_delimitor = default_line_delimitor);

bool host_is_little_endian();



}

#endif