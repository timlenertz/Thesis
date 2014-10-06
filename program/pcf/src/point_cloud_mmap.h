#ifndef PCF_POINT_CLOUD_MMAP_H_
#define PCF_POINT_CLOUD_MMAP_H_

#include <string>
#include <cassert>
#include <unistd.h>
#include <boost/iostreams/device/mapped_file.hpp>
#include "point_cloud.h"

namespace pcf {

template<typename Point>
class point_cloud_mmap : public point_cloud<Point> {
private:
	using super = point_cloud<Point>;

	const bool temporary_;
	const std::string path_;
	boost::iostreams::mapped_file file_;
	
public:
	point_cloud_mmap(std::size_t size, const std::string& path, bool temporary = true) :
	temporary_(temporary), path_(path) {
		// TODO move into cc
		assert(boost::iostreams::mapped_file::alignment() % alignof(Point) == 0);
		
		boost::iostreams::mapped_file_params params(path_);
		params.flags = boost::iostreams::mapped_file::readwrite;
		params.new_file_size = size * sizeof(Point);
		
		file_.open(params);
		
		super::buffer_ = reinterpret_cast<Point*>(file_.data());
		super::buffer_end_ = super::buffer_ + size;
		super::check_correct_alignment_();
	}
	
	~point_cloud_mmap() override {
		file_.close();
		if(temporary_) unlink(path_.c_str());
	}
};

}

#endif