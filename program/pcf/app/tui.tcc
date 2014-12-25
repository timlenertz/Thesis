#include <sstream>
#include <stdexcept>

namespace pcf {

template<typename T>
T read_from_string(const std::string& str) {
	std::stringstream sstr(str, std::stringstream::in);
	sstr.exceptions(std::stringstream::failbit);
	T value;
	sstr >> value;
	if(sstr.good() || sstr.eof()) return value;
	else throw std::runtime_error("Could not read value from string.");
}


template<typename T>
T read_from_string(const std::string& str, const T& def) {
	std::stringstream sstr(str, std::stringstream::in);
	T value;
	sstr >> value;
	if(sstr.good() || sstr.eof()) return value;
	else return def;
}


template<typename T>
T read_from_input(const std::string& prompt) {
	std::cout << prompt << ": ";

	std::string line;
	do {
		std::getline(std::cin, line);
	} while(line.empty());
	
	T value = read_from_string<T>(line);
	std::cout << "Using " << value << std::endl;
	return value;
}


template<typename T>
T read_from_input(const std::string& prompt, const T& def) {
	std::cout << prompt << " (default: " << def << "): ";

	std::string line;
	std::getline(std::cin, line);
	if(! line.empty()) {
		T value = read_from_string<T>(line);
		std::cout << "Using " << value << std::endl;
		return value;
	} else {
		std::cout << "Using default " << def << std::endl;
		return def;
	}
}



template<typename T, typename... Others>
void print(const T& val, const Others&... others) {
	std::cout << val;
	print(others...);
}


template<typename T>
void print(const T& val) {
	std::cout << val << std::endl;
}



}