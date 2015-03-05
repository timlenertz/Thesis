#include "experiment.h"
#include <stdexcept>

namespace pcf {

void experiment::run_registration_(const fixed_point_cloud_type& fixed, loose_point_cloud_type& loose) const {
	pose old_pose = loose.relative_pose();
	auto reg = create_registration(fixed, loose);
	reg->run();
	delete reg;
	loose.set_relative_pose(old_pose);
}


void experiment::run() {		
	for(unsigned i = 0; i < fixed_modifier_runs; ++i) {
		unorganized_point_cloud_full fixed_unorg = original_point_cloud;
		if(fixed_modifier) fixed_modifier(fixed_unorg);
		kdtree_point_cloud_full fixed(std::move(fixed_unorg));
		
		for(unsigned j = 0; j < loose_modifier_runs; ++j) {
			unorganized_point_cloud_full loose = original_point_cloud;
			if(loose_modifier) loose_modifier(loose);
			
			for(unsigned k = 0; k < displacer_runs; ++k) {
				if(displacer) displacer(loose);
				
				for(unsigned l = 0; l < registration_runs; ++l) {
					run_registration_(fixed, loose);
				}
			}
		}
	}
}

}
