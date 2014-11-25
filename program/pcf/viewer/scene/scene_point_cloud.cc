#include "scene_point_cloud.h"
#include "../shader_program.h"
#include "../shader.h"
#include <cstddef>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <stdexcept>

namespace pcf {


class scene_point_cloud::loader {
public:
	struct request {
		camera cam;
		point_full* buffer;
		std::size_t buffer_capacity;
	};
	
	struct response {
		std::size_t size;
	};
	
	
private:
	const pov_point_cloud_full& point_cloud_;

	std::thread thread_;
	std::mutex wake_up_mut_;
	std::condition_variable wake_up_cv_;

	std::atomic_bool should_wake_up_ = false;
	std::atomic_bool running_ = false;

	std::mutex accessing_request_mut_;
	request next_request_;
	
	bool response_available_ = false;
	response response_;
	
	void thread_main_();
	void thread_iteration_();
	
public:
	loader(const pov_point_cloud_full&);
	
	void set_next_request(const request&);
	bool response_available() const;
	bool take_response(response&);
};


scene_point_cloud::loader::loader(const pov_point_cloud_full& pc) :
point_cloud_(pc) {
	thread_ = std::thread(&loader::thread_main_, this);
}
	


void scene_point_cloud::loader::thread_main_() {
	for(;;) thread_iteration_();
}


void scene_point_cloud::loader::thread_iteration_() {
	// Wait until requested to wake up
	std::unique_lock<std::mutex> lk(wake_up_mut_);
	wake_up_cv_.wait(lk, []{ return should_wake_up_; });
	should_wake_up_ = false;
	running_ = true;
	
	// Read request
	accessing_request_mut_.lock();
	request req = next_request_;
	accessing_request_mut_.unlock();
	
	// Process request using POV point cloud
	std::size_t sz = point_cloud_.extract(
		req.buffer,
		req.buffer_capacity,
		req.cam
	);
	
	// Make response available
	response.size = sz;
	response_available_ = true;
	running_ = false;
}


void scene_point_cloud::loader::set_next_request(const request& req) {
	accessing_request_mut_.lock();
	next_request_ = req;
	accessing_request_mut_.unlock();
	
	should_wake_up_ = true;
	wake_up_cv_.notify_one();
}


bool scene_point_cloud::loader::response_available() const {
	if(running_) return false;
	else return response_available_;
}


bool scene_point_cloud::loader::take_response(response& rsp) {
	if(running_) return false;
	if(! response_available_) return false;
	
	rsp = repsonse_;
	response_available_ = false;
	return true;
}




shader_program* scene_point_cloud::shader_program_ = nullptr;


void scene_point_cloud::setup_loader_() {
	loader_.reset( new loader(point_cloud_s) );
}


void scene_point_cloud::take_loader_reponse_() {
	loader::response rsp;
	if(loader_->take_response(rsp)) {
		// Loader has outputted new point set into loader buffer.
		glBindBuffer(GL_ARRAY_BUFFER, loader_point_buffer_);
		
		// Unmap the loader buffer
		bool valid = glUnmapBuffer(GL_ARRAY_BUFFER);
		
		if(valid) {
			// Swap loader and renderer buffers
			renderer_point_buffer_size_ = 0;
			std::swap(loader_point_buffer_, renderer_point_buffer_);
			renderer_point_buffer_size_ = rep.size;
		} else {
			// Buffer has become invalid; reinitialize it
			glDeleteBuffers(1, &loader_point_buffer_);
			glGenBuffers(1, &loader_point_buffer_);
			glBindBuffer(GL_ARRAY_BUFFER, loader_point_buffer_);
			glBufferData(GL_ARRAY_BUFFER, point_buffer_capacity_*sizeof(point_full), NULL, GL_DYNAMIC_DRAW);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		// Assign new renderer buffer to VOA.
		update_vertex_array_object_buffer_();
		
		// Map new loader buffer.
		glBindBuffer(GL_ARRAY_BUFFER, loader_point_buffer_);
		void* buf = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		loader_point_buffer_mapping_ = static_cast<point_full*>(buf);
	}
}



void scene_point_cloud::gl_initialize_() {
	// Load shaders for point cloud, if not done so yet
	if(! shader_program_) shader_program_ = new scene_object_shader_program("point_cloud");


	// Allocate point buffers
	glGenBuffers(1, &loader_point_buffer_);
	glBindBuffer(GL_ARRAY_BUFFER, loader_point_buffer_);
	glBufferData(GL_ARRAY_BUFFER, point_buffer_capacity_*sizeof(point_full), NULL, GL_DYNAMIC_DRAW);
	
	glGenBuffers(1, &renderer_point_buffer_);
	glBindBuffer(GL_ARRAY_BUFFER, renderer_point_buffer_);
	glBufferData(GL_ARRAY_BUFFER, point_buffer_capacity_*sizeof(point_full), NULL, GL_DYNAMIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	renderer_point_buffer_size_ = 0;
	
	
	// Setup vertex array object
	glGenVertexArrays(1, &vertex_array_object_);
	update_vertex_array_object_buffer_();


	// Map loader buffer
	glBindBuffer(GL_ARRAY_BUFFER, loader_point_buffer_);
	void* buf = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	loader_point_buffer_mapping_ = static_cast<point_full*>(buf);
}


void scene_point_cloud::update_vertex_array_object_buffer_() {
	std::size_t stride = sizeof(point_full);
	std::ptrdiff_t color_offset = offsetof(point_full, color);

	glBindVertexArray(vertex_array_object_);
	glBindBuffer(GL_ARRAY_BUFFER, renderer_point_buffer_);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, stride, 0);
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, stride, reinterpret_cast<const GLvoid*>(color_offset));
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0)
}


void scene_point_cloud::gl_uninitialize_() {
	glBindBuffer(GL_ARRAY_BUFFER, loader_point_buffer_);
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDeleteBuffers(1, &loader_point_buffer_);
	glDeleteBuffers(1, &renderer_point_buffer_);
	
	glDeleteVertexArrays(1, &vertex_array_object_);
}


void scene_point_cloud::gl_draw() {
	// If new data set is available from loader, take it now
	take_loader_reponse_();

	shader_program_->use();
	shader_program_->set_camera(scene_.get_camera());
	
	glBindVertexArray(vertex_array_object_);
	glDrawArrays(GL_POINTS, 0, renderer_point_buffer_size_);
	glBindVertexArray(0);
}


void scene_point_cloud::update_camera(const camera& cam) {
	// Send new request for this camera position to loader,
	// but only if there is no response waiting to be accepted (in gl_draw)
	if(! loader_->response_available()) {
		loader::request req;
		req.cam = cam;
		req.buffer = loader_point_buffer_mapping_;
		req.capacity = point_buffer_capacity_;
		loader_->set_next_request(req);
	}
}


}
