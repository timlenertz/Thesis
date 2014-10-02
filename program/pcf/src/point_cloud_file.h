#ifndef PCF_POINT_CLOUD_FILE_H_
#define PCF_POINT_CLOUD_FILE_H_

#include <boost/interprocess/file_mapping.hpp>
#include "point_cloud.h"

namespace pcf {

template<typename Point>
class point_cloud_file : public point_cloud<Point> {
private:
	boost::interprocess::file_mapping mapping_;
	boost::interprocess::mapped_region region_;
	
public:
	explicit point_cloud_file(const char* filename, boost::interprocess::mode_t mode = boost::interprocess::read_write) :
	mapping_(filename, mode), region_(mapping_, mode) {
		buffer_ = region_.get_address();
		buffer_end_ = buffer_ + region_.get_size();
	}
};

}

#endif