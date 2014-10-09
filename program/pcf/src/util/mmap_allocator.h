#ifndef PCF_MMAP_ALLOCATOR_H_
#define PCF_MMAP_ALLOCATOR_H_

#include <string>
#include <cassert>
#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/filesystem.hpp>

namespace pcf {

template<class T>
class mmap_allocator {
public:
	using value_type = T;

private:
	const bool temporary_;
	const boost::filesystem::path path_;
	boost::iostreams::mapped_file file_;

public:
	mmap_allocator(const boost::filesystem::path& path = boost::filesystem::path(), bool temporary = true) :
	path_(path.empty() ? boost::filesystem::unique_path() : path), temporary_(temporary) { }

	T* allocate(std::size_t n, const T* hint = 0) {
		assert(file_.alignment() % alignof(T) == 0);
	
		boost::iostreams::mapped_file_params params(path_.native());
		params.flags = boost::iostreams::mapped_file::readwrite;
		params.new_file_size = n * sizeof(T);
		params.hint = 0;
		
		file_.open(params);
		
		return reinterpret_cast<T*>(file_.data());
	}
	
	void deallocate(T* buf, std::size_t n) {
		file_.close();
		if(temporary_) boost::filesystem::remove(path_);
	}	
};

}

#endif
