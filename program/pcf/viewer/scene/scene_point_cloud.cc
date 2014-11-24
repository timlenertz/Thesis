#include "scene_point_cloud.h"
#include "../shader_program.h"
#include "../shader.h"
#include <cstddef>

namespace pcf {

shader_program* scene_point_cloud::shader_program_ = nullptr;


void scene_point_cloud::gl_setup_position_vertex_attribute_(GLuint ind) {
	std::size_t stride = sizeof(point_full);
	glVertexAttribPointer(ind, 4, GL_FLOAT, GL_FALSE, stride, 0);
}


void scene_point_cloud::gl_setup_color_vertex_attribute_(GLuint ind) {
	std::size_t stride = sizeof(point_full);
	std::ptrdiff_t offset = offsetof(point_full, color);
	// offsetof: might not work, point_full is not standard layout
	glVertexAttribPointer(ind, 3, GL_UNSIGNED_BYTE, GL_TRUE, stride, reinterpret_cast<const GLvoid*>(offset));
}


void scene_point_cloud::gl_initialize_() {
	// Load shaders for point cloud, if not done so yet
	if(! shader_program_) shader_program_ = new shader_program({
		shader::from_file(GL_VERTEX_SHADER, "shaders/point_cloud.vert"),
		shader::from_file(GL_FRAGMENT_SHADER, "shaders/point_cloud.frag")
	});


	// Allocate point buffers
	glGenBuffers(1, &loader_point_buffer_);
	glBindBuffer(GL_ARRAY_BUFFER, loader_point_buffer_);
	glBufferData(GL_ARRAY_BUFFER, point_buffer_capacity_*sizeof(point_full), NULL, GL_DYNAMIC_DRAW);
	
	glGenBuffers(1, &renderer_point_buffer_);
	glBindBuffer(GL_ARRAY_BUFFER, renderer_point_buffer_);
	glBufferData(GL_ARRAY_BUFFER, point_buffer_capacity_*sizeof(point_full), NULL, GL_DYNAMIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void scene_point_cloud::gl_uninitialize_() {
	glDeleteBuffers(1, &loader_point_buffer_);
	glDeleteBuffers(1, &renderer_point_buffer_);
}


void scene_point_cloud::gl_draw() {
	shader_program_->use();
	

	glBindBuffer(GL_ARRAY_BUFFER, renderer_point_buffer_);
	glDrawArrays(GL_POINTS, 0, renderer_point_buffer_size_);
}


}
