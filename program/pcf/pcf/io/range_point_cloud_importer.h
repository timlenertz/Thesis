#ifndef PCF_RANGE_POINT_CLOUD_IMPORTER_H_
#define PCF_RANGE_POINT_CLOUD_IMPORTER_H_

#include "point_cloud_importer.h"

namespace pcf {

class range_point_cloud_importer : public point_cloud_importer {
public:
	virtual std::size_t width() const = 0;
	virtual std::size_t height() const = 0;

	std::size_t size() const final override;
	bool all_valid() const final override;
		
	void read_row(point_xyz*);
	void read_row(point_full*);
};

}

#endif