#ifndef PCFVW_SCENE_PLANE_H_
#define PCFVW_SCENE_PLANE_H_

#include "object.h"
#include "../../pcf_core/geometry/plane.h"
#include "../../pcf_core/rgb_color.h"
#include "../../pcf_core/space_object_wrapper.h"
#include "../gl.h"
#include <utility>

namespace pcf {

class scene_object_shader_program;

class scene_plane : public scene_object {
private:
	static scene_object_shader_program* shader_program_;
	
	GLuint edges_vertex_buffer_;
	GLuint edges_vertex_array_object_;
	
	GLuint faces_vertex_buffer_;
	GLuint faces_vertex_array_object_;
	
	void initialize_();
	
	const Eigen::Matrix4f matrix_;
	
protected:
	void gl_initialize_() override;
	void gl_uninitialize_() override;
	void gl_draw_() override;

public:
	rgb_color edges_color = rgb_color::white;
	bool draw_faces = true;
	rgb_color faces_color = rgb_color::blue;
	float faces_alpha = 0.3;
	float side_length = 1.0;

	scene_plane(const scene& sc, const space_plane& pl, const rgb_color& edge_col = rgb_color::white) :
		scene_object(sc), matrix_(pl->to_pose().transformation_to_world().matrix()), edges_color(edge_col), draw_faces(true) { }
	scene_plane(const scene& sc, const space_plane& pl, const rgb_color& edge_col, const rgb_color& face_col) :
		scene_object(sc), matrix_(pl->to_pose().transformation_to_world().matrix()), edges_color(edge_col), draw_faces(true), faces_color(face_col) { }		
	~scene_plane();
	
	void take_parameters(const scene_plane& p) {
		edges_color = p.edges_color;
		draw_faces = p.draw_faces;
		faces_color = p.faces_color;
		faces_alpha = p.faces_alpha;
		side_length = p.side_length;
	}
};

}

#endif