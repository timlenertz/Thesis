#ifndef PCF_ASSET_H_
#define PCF_ASSET_H_

#include <Eigen/Eigen>
#include <Eigen/Geometry>

namespace pcf {

/**
Asset of data that represents part of the scene.
Kept in working memory and modified during operations. Can be point cloud, range image, photo...
*/
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