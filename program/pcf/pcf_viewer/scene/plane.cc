#include "plane.h"
#include "shader_program.h"
#include "scene.h"

namespace {
	#include "../shaders/single_color.frag.h"
	#include "../shaders/single_color.vert.h"
}

namespace pcf {

scene_object_shader_program* scene_plane::shader_program_ = nullptr;

void scene_plane::gl_initialize_() {
	if(! shader_program_)
		shader_program_ = new scene_object_shader_program(
			std::string((const char*)shaders_single_color_vert, shaders_single_color_vert_len),
			std::string((const char*)shaders_single_color_frag, shaders_single_color_frag_len)
		);

	initialize_();
}


void scene_plane::initialize_() {
	const GLfloat edge_vertices[4][2][3] = {
		 // two points per line of square
		{ {-1, 0, -1}, {+1, 0, -1} },
		{ {+1, 0, -1}, {+1, 0, +1} },
		{ {+1, 0, +1}, {-1, 0, +1} },
		{ {-1, 0, +1}, {-1, 0, -1} }
	};
	const GLfloat face_vertices[2][3][3] = {
		// one point per corner, two triangles
		{ {-1, 0, -1}, {-1, 0, +1}, {+1, 0, +1} },
		{ {-1, 0, -1}, {+1, 0, -1}, {+1, 0, +1} }
	};
	
	// Generate OpenGL vertex buffers
	glGenBuffers(1, &edges_vertex_buffer_);
	glGenBuffers(1, &faces_vertex_buffer_);

	glBindBuffer(GL_ARRAY_BUFFER, edges_vertex_buffer_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(edge_vertices), (const GLvoid*)edge_vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, faces_vertex_buffer_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(face_vertices), (const GLvoid*)face_vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	// Setup vertex array objects
	glGenVertexArrays(1, &edges_vertex_array_object_);
	glGenVertexArrays(1, &faces_vertex_array_object_);

	glBindVertexArray(edges_vertex_array_object_);
	glBindBuffer(GL_ARRAY_BUFFER, edges_vertex_buffer_);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glBindVertexArray(faces_vertex_array_object_);
	glBindBuffer(GL_ARRAY_BUFFER, faces_vertex_buffer_);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


void scene_plane::gl_uninitialize_() {
	glDeleteBuffers(1, &edges_vertex_buffer_);	
	glDeleteVertexArrays(1, &edges_vertex_array_object_);
	glDeleteBuffers(1, &faces_vertex_buffer_);	
	glDeleteVertexArrays(1, &faces_vertex_array_object_);
}


void scene_plane::gl_draw_() {
	Eigen::Affine3f scaling(Eigen::Scaling(side_length));

	shader_program_->use();
	shader_program_->mvp_matrix = Eigen::Matrix4f(mvp_matrix_ * matrix_ * scaling.matrix());
	
	if(draw_faces) {
		glBindVertexArray(faces_vertex_array_object_);
		shader_program_->uniform("color") = faces_color;
		shader_program_->uniform("alpha") = faces_alpha;
		glDrawArrays(GL_TRIANGLES, 0, 3 * 2);
	}
	
	glBindVertexArray(edges_vertex_array_object_);
	shader_program_->uniform("color") = edges_color;
	shader_program_->uniform("alpha") = 1.0;
	glDrawArrays(GL_LINES, 0, 4 * 2);
	
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

		
scene_plane::~scene_plane() { }


}