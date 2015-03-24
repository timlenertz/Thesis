#ifndef PCFVW_SCENE_BOUNDING_BOX_H_
#define PCFVW_SCENE_BOUNDING_BOX_H_

#include "object.h"
#include "../../pcf_core/geometry/bounding_box.h"
#include "../../pcf_core/rgb_color.h"
#include "../../pcf_core/space_object_wrapper.h"
#include "../gl.h"
#include <utility>

namespace pcf {

class scene_object_shader_program;

class scene_bounding_box : public scene_object {
private:
	static scene_object_shader_program* shader_program_;
	
	GLuint edges_vertex_buffer_;
	GLuint edges_vertex_array_object_;
	
	GLuint faces_vertex_buffer_;
	GLuint faces_vertex_array_object_;
	
	void initialize_edges_();
	void initialize_faces_();
	
protected:
	void gl_initialize_() override;
	void gl_uninitialize_() override;
	void gl_draw_() override;

public:
	const bounding_box box;
	rgb_color edges_color = rgb_color::white;
	bool draw_faces = false;
	rgb_color faces_color = rgb_color::red;
	float faces_alpha = 0.3;

	scene_bounding_box(const scene& sc, const space_bounding_box& b, const rgb_color& edge_col = rgb_color::yellow) :
		scene_object(sc), box(*b), edges_color(edge_col), draw_faces(false) { }
	scene_bounding_box(const scene& sc, const space_bounding_box& b, const rgb_color& edge_col, const rgb_color& face_col) :
		scene_object(sc), box(*b), edges_color(edge_col), draw_faces(true), faces_color(face_col) { }		
	~scene_bounding_box();
	
	void take_parameters(const scene_bounding_box& b) {
		edges_color = b.edges_color;
		draw_faces = b.draw_faces;
		faces_color = b.faces_color;
		faces_alpha = b.faces_alpha;
	}
};

}

#endif