#include "registration_correspondences.h"
#include "shader_program.h"
#include "scene.h"
#include <memory>
#include <cmath>

#include "../shaders/registration_correspondences.frag.h"
#include "../shaders/registration_correspondences.vert.h"

namespace pcf {

scene_object_shader_program* scene_registration_correspondences::shader_program_ = nullptr;


scene_registration_correspondences::scene_registration_correspondences(const scene& sc, const space_registration_correspondences_list& lst) :
scene_object(sc) {
	number_of_correspondences_ = lst->size();
	std::size_t n = number_of_correspondences_ * (2 * 4);
	pending_data_.reset(new GLfloat[n]);
	GLfloat* it = pending_data_.get();
	min_weight_ = +INFINITY;
	max_weight_ = 0.0;
	for(const registration_correspondence& c : *lst) {
		float w = c.weight;
		if(w < min_weight_) min_weight_ = w;
		if(w > max_weight_) max_weight_ = w;
	
		for(std::ptrdiff_t i = 0; i < 3; ++i) *(it++) = c.fixed[i];
		*(it++) = w;
		for(std::ptrdiff_t i = 0; i < 3; ++i) *(it++) = c.loose[i];
		*(it++) = w;
	}
}


void scene_registration_correspondences::gl_initialize_() {
	if(! shader_program_)
		shader_program_ = new scene_object_shader_program(
			std::string((const char*)shaders_registration_correspondences_vert, shaders_registration_correspondences_vert_len),
			std::string((const char*)shaders_registration_correspondences_frag, shaders_registration_correspondences_frag_len)
		);

	initialize_edges_();
}


void scene_registration_correspondences::initialize_edges_() {
	// Generate OpenGL vertex buffer
	std::size_t sz = number_of_correspondences_ * 2 * 4 * sizeof(float);
	glGenBuffers(1, &edges_vertex_buffer_);
	glBindBuffer(GL_ARRAY_BUFFER, edges_vertex_buffer_);
	glBufferData(GL_ARRAY_BUFFER, sz, (const GLvoid*)pending_data_.get(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	pending_data_.reset();
	
	// Setup vertex array object
	glGenVertexArrays(1, &edges_vertex_array_object_);
	glBindVertexArray(edges_vertex_array_object_);
	glBindBuffer(GL_ARRAY_BUFFER, edges_vertex_buffer_);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 16, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 16, reinterpret_cast<const GLvoid*>(12));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void scene_registration_correspondences::gl_uninitialize_() {
	glDeleteBuffers(1, &edges_vertex_buffer_);	
	glDeleteVertexArrays(1, &edges_vertex_array_object_);
}


void scene_registration_correspondences::gl_draw_() {
	shader_program_->use();
	shader_program_->mvp_matrix = mvp_matrix_;
	shader_program_->uniform("min_edge_color") = min_edge_color;
	shader_program_->uniform("max_edge_color") = max_edge_color;
	shader_program_->uniform("min_edge_alpha") = min_edge_alpha;
	shader_program_->uniform("max_edge_alpha") = max_edge_alpha;
	shader_program_->uniform("min_weight") = min_weight_;
	shader_program_->uniform("max_weight") = max_weight_;
	
	glBindVertexArray(edges_vertex_array_object_);
	glDrawArrays(GL_LINES, 0, number_of_correspondences_ * 2);
	
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


}