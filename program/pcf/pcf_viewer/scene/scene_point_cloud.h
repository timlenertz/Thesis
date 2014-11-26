#ifndef PCFVW_SCENE_POINT_CLOUD_H_
#define PCFVW_SCENE_POINT_CLOUD_H_

#include "scene_object.h"
#include <pcf/point_cloud/pov/pov_point_cloud.h>
#include <utility>
#include <loader>

namespace pcf {

class scene_object_shader_program;

class scene_point_cloud : public scene_object {
private:
	class loader;
	
private:
	pov_point_cloud_full point_cloud_;
	const GLsizei point_buffer_capacity_ = 256 * 1024;	

	std::unique_ptr<loader> loader_;
	
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
	void gl_draw() override;

public:
	template<typename Cloud>
	explicit scene_point_cloud(Cloud&& pc) :
		point_cloud_(std::forward<Cloud>(pc)) { setup_loader_(); }
		
	void update_camera(const camera&) override;
		
};

}

#endif