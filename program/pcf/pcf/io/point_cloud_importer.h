#ifndef PCF_POINT_CLOUD_IMPORTER_H_
#define PCF_POINT_CLOUD_IMPORTER_H_

#include "../point.h"
#include "../geometry/pose.h"

namespace pcf {

class point_cloud_importer {
protected:
	point_cloud_importer() = default;
	point_cloud_importer(const point_cloud_importer&) = delete;
	point_cloud_importer& operator=(const point_cloud_importer&) = delete;
	
public:
	virtual ~point_cloud_importer() { }
	
	virtual std::size_t size() const = 0;
	virtual bool all_valid() const = 0;
	
	virtual bool has_camera_pose() const;
	virtual pose camera_pose() const;
	
	virtual void rewind() = 0;
	virtual std::ptrdiff_t tell() const = 0;
	virtual void read(point_xyz*, std::size_t sz) = 0;
	virtual void read(point_full*, std::size_t sz) = 0;
};

}

#endif