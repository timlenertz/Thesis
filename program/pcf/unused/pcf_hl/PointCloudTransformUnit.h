#ifndef PCFHL_POINT_CLOUD_TRANSFORM_UNIT_H_
#define PCFHL_POINT_CLOUD_TRANSFORM_UNIT_H_

#include "Unit.h"

namespace pcf {

class unorganized_point_cloud_full;

class PointCloudTransformUnit : public TransformUnit {
private:
	const bool need_unorganized_;

protected:
	explicit PointCloudTransformUnit(bool need_unorg = false) :
		need_unorganized_(need_unorg) { }

	virtual void run_with_point_cloud_(unorganized_point_cloud_full&) = 0;
	virtual void run_with_point_cloud_(point_cloud_full&) = 0;

	void make_unorganized_();

public:
	void run() final override;
};


}

#endif