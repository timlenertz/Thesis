#ifndef PCFAPP_TUI_H_
#define PCFAPP_TUI_H_

#include <iostream>
#include <string>
#include <vector>

namespace pcf {

using choices = std::vector<std::string>;

template<typename T> T read_from_string(const std::string& str);
template<typename T> T read_from_string(const std::string& str, const T& def);
template<typename T> T read_from_input(const std::string& prompt);
template<typename T> T read_from_input(const std::string& prompt, const T& def);
std::string read_line(const std::string& prompt, const std::string& def = std::string(""));

template<typename T, typename... Others> void print(const T&, const Others&...);
template<typename T> void print(const T&);

int read_choice(const std::string& prompt, const choices&);


}

#include "tui.tcc"

#endif