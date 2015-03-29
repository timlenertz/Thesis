#ifndef PCFVW_SCENE_REGISTRATION_CORRESPONDENCES_H_
#define PCFVW_SCENE_REGISTRATION_CORRESPONDENCES_H_

#include "object.h"
#include "../../pcf_core/registration/correspondences/registration_correspondence.h"
#include "../../pcf_core/registration/correspondences/registration_correspondences_list.h"
#include "../../pcf_core/rgb_color.h"
#include "../gl.h"
#include <utility>
#include <deque>

namespace pcf {

class scene_object_shader_program;

class scene_registration_correspondences : public scene_object {
private:
	static scene_object_shader_program* shader_program_;
	
	GLuint edges_vertex_buffer_;
	GLuint edges_vertex_array_object_;
	
	void initialize_edges_();
	
protected:
	void gl_initialize_() override;
	void gl_uninitialize_() override;
	void gl_draw_() override;

private:
	std::unique_ptr<GLfloat[]> pending_data_;
	std::size_t number_of_correspondences_;
	float min_weight_;
	float max_weight_;

public:
	rgb_color min_edge_color = rgb_color::white;
	rgb_color max_edge_color = rgb_color::yellow;
	float min_edge_alpha = 0.3;
	float max_edge_alpha = 1.0;

	scene_registration_correspondences(const scene& sc, const space_registration_correspondences_list&);
};


}

#endif