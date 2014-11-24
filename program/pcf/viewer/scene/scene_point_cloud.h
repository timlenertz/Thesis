#ifndef PCFVW_SCENE_POINT_CLOUD_H_
#define PCFVW_SCENE_POINT_CLOUD_H_

#include "scene_object.h"
#include <utility>
#include <pcf/point_cloud/pov/pov_point_cloud.h>

namespace pcf {

class shader_program;

class scene_point_cloud : public scene_object {
private:
	static shader_program* shader_program_;
	
	pov_point_cloud_full point_cloud_;
	
	const GLsizei point_buffer_capacity_ = 256 * 1024;
	GLuint loader_point_buffer_;
	GLuint renderer_point_buffer_;
	GLsizei renderer_point_buffer_size_;

	static void gl_setup_position_vertex_attribute_(GLuint);
	static void gl_setup_color_vertex_attribute_(GLuint);

protected:
	void gl_initialize_() override;
	void gl_uninitialize_() override;
	void gl_draw() override;

public:
	template<typename Cloud>
	explicit scene_point_cloud(Cloud&& pc) :
		point_cloud_(std::forward<Cloud>(pc)) { }
};

}

#endif