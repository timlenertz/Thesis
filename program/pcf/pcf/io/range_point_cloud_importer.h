#ifndef PCF_RANGE_POINT_CLOUD_IMPORTER_H_
#define PCF_RANGE_POINT_CLOUD_IMPORTER_H_

#include "point_cloud_importer.h"
#include "../geometry/spherical_coordinates.h"

namespace pcf {

class range_point_cloud_importer : public point_cloud_importer {
public:
	virtual std::size_t rows() const = 0;
	virtual std::size_t columns() const = 0;
	virtual void read_spherical(spherical_coordinates*, std::size_t sz) = 0;

	std::size_t size() const final override;
	bool all_valid() const final override;
		
	void read_row(point_xyz*);
	void read_row(point_full*);
	void read_row_spherical(spherical_coordinates*);
};

}

#endif