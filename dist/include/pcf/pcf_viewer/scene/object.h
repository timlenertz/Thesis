#ifndef PCFVW_SCENE_OBJECT_H_
#define PCFVW_SCENE_OBJECT_H_

#include "../gl_object.h"
#include "../../pcf_core/geometry/pose.h"
#include "../../pcf_core/space_object.h"

namespace pcf {

class projection_camera;
class scene;

/**
Abstract base class for scene object.
Handles computation of MVP matrix based on object and camera pose. Subclass handles rendering of object.
*/
class scene_object : public gl_object {
protected:
	const scene& scene_;
	Eigen::Affine3f model_transformation_;
	Eigen::Matrix4f mvp_matrix_;	
		
	explicit scene_object(const scene&);
					
	virtual void mvp_was_updated_();

private:
	void compute_mvp_matrix_();

	scene_object(const scene_object&) = delete;

public:
	void handle_camera_update();
	
	void set_model_transformation(const Eigen::Affine3f&);
	const Eigen::Affine3f& model_transformation() const { return model_transformation_; }
	
	const scene& get_scene() const { return scene_; }
	
	bool visible() const;
	void set_visible(bool);
};

}

#endif