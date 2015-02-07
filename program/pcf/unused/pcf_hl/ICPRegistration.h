#ifndef PCFHL_ICP_REGISTRATION_H_
#define PCFHL_ICP_REGISTRATION_H_

#include "../PointCloudTransformUnit.h"

namespace pcf {

class ICPRegistration : public PointCloudTransformUnit {
private:
	SpaceObjectPtr fixed_;
	
public:
	ICPRegistration();
	
	void set_fixed(SpaceObjectPtr pc) { fixed_ = pc; }
	SpaceObjectPtr get_fixed() const { return fixed_; }

	void run_with_point_cloud_(unorganized_point_cloud_full&) override;
};


}

#endif