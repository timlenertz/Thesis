#include <sstream>
#include <typeinfo>

namespace pcf {

template<typename T>
std::string to_string(const T& t) {
	std::ostringstream str;
	str << t;
	return str.str();
}


template<typename T>
T from_string(const std::string& s) {
	std::istringstream str(s);
	T t;
	str >> t;
	if(str.fail()) throw std::invalid_argument("Cannot convert \"" + s + "\" to type " + typeid(T).name() + ".");
	else return t;
}


template<typename T>
std::vector<T> explode_from_string(char separator, const std::string& str) {
	std::vector<std::string> vec = explode(separator, str);
	std::vector<T> vec2(vec.size());
	auto it2 = vec2.begin();
	for(const std::string& s : vec) *(it2++) = from_string<T>(s);
	return vec2;
}


template<typename T>
std::string implode_to_string(char separator, const std::vector<T>& vec) {
	std::vector<std::string> vec2(vec.size());
	auto it2 = vec2.begin();
	for(const T& t : vec) *(it2++) = to_string(t);
	return implode(separator, vec2);
}


}