#ifndef PCFVW_SCENE_POINT_CLOUD_H_
#define PCFVW_SCENE_POINT_CLOUD_H_

#include "object.h"
#include "../point_cloud/pov_point_cloud.h"
#include "../gl.h"
#include <utility>

namespace pcf {

class scene_object_shader_program;

/**
Point cloud scene object.
Only renders parts visible from current view point using the underlying pov_point_cloud. Downsamples when necessary so that the rendered part fits into given capacity. Visible point set extraction occurs in separate thread managed by this object, buffer swap mechanism is used to allow for uninterrupted rendering.
*/
class scene_point_cloud : public scene_object {
private:
	static const GLsizei default_point_buffer_capacity_;

	class loader;
	
	const GLsizei point_buffer_capacity_;
	pov_point_cloud_full point_cloud_;

	loader* loader_ = nullptr;
	
	static scene_object_shader_program* shader_program_;
		
	GLuint loader_point_buffer_;
	GLuint renderer_point_buffer_;
	GLsizei renderer_point_buffer_size_;
	point_full* loader_point_buffer_mapping_;
	
	GLuint vertex_array_object_;
	
	void setup_loader_();
	void take_loader_reponse_();
	void update_vertex_array_object_buffer_();

protected:
	void gl_initialize_() override;
	void gl_uninitialize_() override;
	void gl_draw_() override;

public:
	template<typename Cloud>
	scene_point_cloud(const scene& sc, Cloud&& pc, GLsizei cap = default_point_buffer_capacity_) :
		scene_object(sc, pc.absolute_pose),
		point_buffer_capacity_(cap),
		point_cloud_(std::forward<Cloud>(pc)) { setup_loader_(); }
			
	~scene_point_cloud();
		
	void updated_camera_or_pose_() override;
	
	pov_point_cloud_full& operator*() { return point_cloud_; }
	const pov_point_cloud_full& operator*() const { return point_cloud_; }
	pov_point_cloud_full* operator->() { return &point_cloud_; }
	const pov_point_cloud_full* operator->() const { return &point_cloud_; }
};

}

#endif