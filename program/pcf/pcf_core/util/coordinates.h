#ifndef PCF_COORDINATES_H_
#define PCF_COORDINATES_H_

#include <array>
#include <ostream>
#include <initializer_list>
#include <Eigen/Eigen>

namespace pcf {

template<std::size_t Dim, typename T>
class coordinates {
private:
	std::array<T, Dim> coord_;

public:
	template<typename... Arg>
	explicit coordinates(const Arg&... arg) : 
		coord_({ arg... }) { }
	
	coordinates(std::initializer_list<T> coord) :
		coord_(coord) { }
	
	coordinates() { coord_.fill(T()); }
	
	coordinates(const coordinates&) = default;
	coordinates& operator=(const coordinates&) = default;
	
	bool operator==(const coordinates& c) const {
		for(std::ptrdiff_t i = 0; i < Dim; ++i)
			if(coord_[i] != c.coord_[i]) return false;
		return true;
	};
	bool operator!=(const coordinates& c) const {
		return ! operator==(c);
	}
	
	static std::size_t size() { return Dim; }
	
	const T& operator[](std::ptrdiff_t i) const { return coord_[i]; }
	T& operator[](std::ptrdiff_t i) { return coord_[i]; }
	
	using iterator = T*;
	using const_iterator = const T*;
	
	T* begin() { return coord_.begin(); }
	const T* begin() const { return coord_.cbegin(); }
	const T* cbegin() const { return coord_.cbegin(); }
	T* end() { return coord_.end(); }
	const T* end() const { return coord_.cend(); }
	const T* cend() const { return coord_.cend(); }
};


template<typename T>
class coordinates<2, T> {
public:
	T x;
	T y;
	
	coordinates() :
		x(), y() { }
	coordinates(const T& nx, const T& ny) :
		x(nx), y(ny) { }
	coordinates(std::initializer_list<T> coord) :
		x(*coord.begin()), y(*(coord.begin()+1)) { }
	coordinates(const Eigen::Vector2i& coord) :
		x(coord[0]), y(coord[1]) { }
	coordinates(const coordinates&) = default;
	coordinates& operator=(const coordinates&) = default;
	
	bool operator==(const coordinates& c) const {
		return (x == c.x) && (y == c.y);
	};
	bool operator!=(const coordinates& c) const {
		return ! operator==(c);
	}

	static std::size_t size() { return 2; }
	
	const T& operator[](std::ptrdiff_t i) const { return i ? y : x; }
	T& operator[](std::ptrdiff_t i) { return i ? y : x; }
	
	using iterator = T*;
	using const_iterator = const T*;
	
	T* begin() { return &x; }
	const T* begin() const { return &x; }
	const T* cbegin() const { return &x; }
	T* end() { return &y + 1; }
	const T* end() const { return &y + 1; }
	const T* cend() const { return &y + 1; }
	
	operator Eigen::Vector2i () const { return Eigen::Vector2i(x, y); }
};


template<typename T>
class coordinates<3, T> {
public:
	T x;
	T y;
	T z;
	
	coordinates() :
		x(), y(), z() { }
	coordinates(const T& nx, const T& ny, const T& nz) :
		x(nx), y(ny), z(nz) { }
	coordinates(std::initializer_list<T> coord) :
		x(*coord.begin()), y(*(coord.begin()+1)), z(*(coord.begin()+2)) { }
	coordinates(const Eigen::Vector3i& coord) :
		x(coord[0]), y(coord[1]), z(coord[3]) { }
	coordinates(const coordinates&) = default;
	coordinates& operator=(const coordinates&) = default;
	
	bool operator==(const coordinates& c) const {
		return (x == c.x) && (y == c.y) && (z == c.z);
	};
	bool operator!=(const coordinates& c) const {
		return ! operator==(c);
	}
	
	static std::size_t size() { return 3; }
	
	const T& operator[](std::ptrdiff_t i) const {
		switch(i) {
			case 0: return x;
			case 1: return y;
			default: return z;
		}
	}
	T& operator[](std::ptrdiff_t i) {
		switch(i) {
			case 0: return x;
			case 1: return y;
			default: return z;
		}
	}

	using iterator = T*;
	using const_iterator = const T*;
	
	T* begin() { return &x; }
	const T* begin() const { return &x; }
	const T* cbegin() const { return &x; }
	T* end() { return &z + 1; }
	const T* end() const { return &z + 1; }
	const T* cend() const { return &z + 1; }	
	
	operator Eigen::Vector3i () const { return Eigen::Vector3i(x, y, z); }
};


template<std::size_t Dim, typename T>
std::ostream& operator<<(std::ostream& str, const coordinates<Dim, T>& coord) {
	str << '(';
	for(std::ptrdiff_t i = 0; i < Dim-1; ++i) str << coord[i] << ", ";
	str << coord[Dim-1] << ')';
	return str;
}


template<std::size_t Dim> using multi_size = coordinates<Dim, std::size_t>;
template<std::size_t Dim> using multi_index = coordinates<Dim, std::ptrdiff_t>;


using size_2dim = multi_size<2>;
using size_3dim = multi_size<3>;
using index_2dim = multi_index<2>;
using index_3dim = multi_index<3>;

}

#endif
