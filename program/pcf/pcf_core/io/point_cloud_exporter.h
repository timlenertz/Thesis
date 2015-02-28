#ifndef PCF_POINT_CLOUD_EXPORTER_H_
#define PCF_POINT_CLOUD_EXPORTER_H_

#include "../point.h"

namespace pcf {

/**
Abstract base class for point cloud exporter.
Subclass implements exporting points into given file format. Subclasses need to handle both point_xyz and point_full.
*/
class point_cloud_exporter {
protected:
	point_cloud_exporter() = default;
	point_cloud_exporter(const point_cloud_exporter&) = delete;
	point_cloud_exporter& operator=(const point_cloud_exporter&) = delete;
	
public:
	virtual ~point_cloud_exporter() { }
	
	virtual void write(const point_xyz*, std::size_t sz) = 0;
	virtual void write(const point_full*, std::size_t sz) = 0;
	virtual void close() = 0;
};

}

#endif