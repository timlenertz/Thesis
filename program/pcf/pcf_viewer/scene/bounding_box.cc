#include "bounding_box.h"
#include "shader_program.h"
#include "scene.h"

#include "../shaders/single_color.frag.h"
#include "../shaders/single_color.vert.h"

namespace pcf {

scene_object_shader_program* scene_bounding_box::shader_program_ = nullptr;

void scene_bounding_box::gl_initialize_() {
	if(! shader_program_)
		shader_program_ = new scene_object_shader_program(
			std::string((const char*)shaders_single_color_vert, shaders_single_color_vert_len),
			std::string((const char*)shaders_single_color_frag, shaders_single_color_frag_len)
		);

	initialize_edges_();
	initialize_faces_();
}


void scene_bounding_box::initialize_edges_() {
	// Setup vertices (two points per line of bounding box)
	GLfloat vertices[12][2][3];
	auto edges = box.edges();
	for(std::ptrdiff_t e = 0; e < 12; ++e) {
		const bounding_box::edge& edge = edges[e];
		for(std::ptrdiff_t i = 0; i < 3; ++i) {
			vertices[e][0][i] = edge.first[i];
			vertices[e][1][i] = edge.second[i];
		}
	}

	// Generate OpenGL vertex buffer
	glGenBuffers(1, &edges_vertex_buffer_);
	glBindBuffer(GL_ARRAY_BUFFER, edges_vertex_buffer_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), (const GLvoid*)vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	// Setup vertex array object
	glGenVertexArrays(1, &edges_vertex_array_object_);
	glBindVertexArray(edges_vertex_array_object_);
	glBindBuffer(GL_ARRAY_BUFFER, edges_vertex_buffer_);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


void scene_bounding_box::initialize_faces_() {
	// Setup vertices (two triangles with three points per face of bounding box)
	GLfloat vertices[6][2][3][3];
	auto faces = box.faces();
	for(std::ptrdiff_t f = 0; f < 6; ++f) {
		const bounding_box::face& face = faces[f];
		
		for(std::ptrdiff_t i = 0; i < 3; ++i) vertices[f][0][0][i] = face[0][i];
		for(std::ptrdiff_t i = 0; i < 3; ++i) vertices[f][0][1][i] = face[1][i];
		for(std::ptrdiff_t i = 0; i < 3; ++i) vertices[f][0][2][i] = face[2][i];
		
		for(std::ptrdiff_t i = 0; i < 3; ++i) vertices[f][1][0][i] = face[0][i];
		for(std::ptrdiff_t i = 0; i < 3; ++i) vertices[f][1][1][i] = face[2][i];
		for(std::ptrdiff_t i = 0; i < 3; ++i) vertices[f][1][2][i] = face[3][i];
	}

	// Generate OpenGL vertex buffer
	glGenBuffers(1, &faces_vertex_buffer_);
	glBindBuffer(GL_ARRAY_BUFFER, faces_vertex_buffer_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), (const GLvoid*)vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	// Setup vertex array object
	glGenVertexArrays(1, &faces_vertex_array_object_);
	glBindVertexArray(faces_vertex_array_object_);
	glBindBuffer(GL_ARRAY_BUFFER, faces_vertex_buffer_);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


void scene_bounding_box::gl_uninitialize_() {
	glDeleteBuffers(1, &edges_vertex_buffer_);	
	glDeleteVertexArrays(1, &edges_vertex_array_object_);
	glDeleteBuffers(1, &faces_vertex_buffer_);	
	glDeleteVertexArrays(1, &faces_vertex_array_object_);
}


void scene_bounding_box::gl_draw_() {
	shader_program_->use();
	shader_program_->mvp_matrix = mvp_matrix_;
	
	if(draw_faces) {
		glBindVertexArray(faces_vertex_array_object_);
		glDepthMask(GL_FALSE);
		shader_program_->uniform("color") = faces_color;
		shader_program_->uniform("alpha") = faces_alpha;
		glDrawArrays(GL_TRIANGLES, 0, 6 * 2 * 3);
		glDepthMask(GL_TRUE);
	}
	
	glBindVertexArray(edges_vertex_array_object_);
	shader_program_->uniform("color") = edges_color;
	shader_program_->uniform("alpha") = 1.0;
	glDrawArrays(GL_LINES, 0, 12 * 2);
	
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

		
scene_bounding_box::~scene_bounding_box() { }


}