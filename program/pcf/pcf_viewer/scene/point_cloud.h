#ifndef PCFVW_SCENE_POINT_CLOUD_H_
#define PCFVW_SCENE_POINT_CLOUD_H_

#include "object.h"
#include "../point_cloud/pov_point_cloud.h"
#include "../gl.h"
#include "../../pcf_core/rgb_color.h"
#include <utility>
#include <memory>

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
	std::unique_ptr<pov_point_cloud_full> point_cloud_;

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
	scene_point_cloud(const scene&, point_cloud_full&, GLsizei cap = default_point_buffer_capacity_);
	scene_point_cloud(const scene&, point_cloud_xyz&, const rgb_color& = rgb_color::white, GLsizei cap = default_point_buffer_capacity_);
			
	~scene_point_cloud();
		
	void object_was_updated_() override;
	void mvp_was_updated_() override;
	
	pov_point_cloud_full& operator*() { return *point_cloud_; }
	const pov_point_cloud_full& operator*() const { return *point_cloud_; }
	pov_point_cloud_full* operator->() { return point_cloud_.get(); }
	const pov_point_cloud_full* operator->() const { return point_cloud_.get(); }
};

}

#endif