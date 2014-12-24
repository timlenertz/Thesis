#include "frustum.h"
#include "shader_program.h"
#include "scene.h"

namespace pcf {

scene_object_shader_program* scene_frustum::shader_program_ = nullptr;

void scene_frustum::gl_initialize_() {
	if(! shader_program_) shader_program_ = new scene_object_shader_program("frustum");

	// Setup vertices (two points per line of bounding box)
	GLfloat vertices[12][2][3];
	auto edges = frustum_.edges();
	for(std::ptrdiff_t e = 0; e < 12; ++e) {
		const frustum::edge& edge = edges[e];
		for(std::ptrdiff_t i = 0; i < 3; ++i) {
			vertices[e][0][i] = edge.first[i];
			vertices[e][1][i] = edge.second[i];
		}
	}

	// Generate OpenGL vertex buffer
	glGenBuffers(1, &vertex_buffer_);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), (const GLvoid*)vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	// Setup vertex array object
	glGenVertexArrays(1, &vertex_array_object_);
	glBindVertexArray(vertex_array_object_);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


void scene_frustum::gl_uninitialize_() {
	glDeleteBuffers(1, &vertex_buffer_);	
	glDeleteVertexArrays(1, &vertex_array_object_);
}


void scene_frustum::gl_draw_() {
	shader_program_->use();
	shader_program_->mvp_matrix = mvp_matrix_;
	shader_program_->uniform("color") = color_;
	
	glBindVertexArray(vertex_array_object_);
	glDrawArrays(GL_LINES, 0, 24);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


scene_frustum::scene_frustum(const scene& sc, const frustum& fr) :
scene_object(sc), frustum_(fr) { }

		
scene_frustum::~scene_frustum() { }


}