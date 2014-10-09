#ifndef PCF_ASSET_H_
#define PCF_ASSET_H_

#include <Eigen/Eigen>
#include <Eigen/Geometry>

namespace pcf {

class asset {
public:
	enum transformation_quality_t { none, coarse, fine };

private:
	Eigen::Affine3f relative_transformation_;
	asset* relative_to_;
	transformation_quality_t transformation_quality_;

public:
	virtual ~asset() { }
};

}

#endif