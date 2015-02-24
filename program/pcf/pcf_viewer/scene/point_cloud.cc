#include "point_cloud.h"
#include "../../pcf_core/util/random.h"
#include "../../pcf_core/point_algorithm.h"
#include "shader_program.h"
#include "scene.h"
#include "../gl.h"
#include <cstddef>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <stdexcept>
#include <chrono>

#include "../shaders/point_cloud.frag.h"
#include "../shaders/point_cloud.vert.h"

#include <iostream>

namespace pcf {

namespace {
	const std::chrono::milliseconds loader_period_(50);
}



class scene_point_cloud::loader {
public:
	struct request {
		frustum fr;
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

	std::atomic_bool should_wake_up_;
	std::atomic_bool running_;
	std::atomic_bool should_exit_;

	std::atomic<request*> next_request_;
	
	bool response_available_ = false;
	response response_;
	
	void thread_main_();
	void thread_iteration_();

	loader(const loader&) = delete;
	loader& operator=(const loader&) = delete;
	
public:
	loader(const pov_point_cloud_full&);
	~loader();
	
	void set_next_request(const request&);
	bool response_available() const;
	bool take_response(response&);
	void exit();
};


scene_point_cloud::loader::loader(const pov_point_cloud_full& pc) :
point_coud_(pc), next_request_(nullptr) {
	running_.store(false);
	should_wake_up_.store(false);
	should_exit_.store(false);
	thread_ = std::thread(&loader::thread_main_, this);
}
	

scene_point_cloud::loader::~loader() {
	exit();
}


void scene_point_cloud::loader::exit() {
	should_exit_ = true;
	should_wake_up_ = true;
	wake_up_cv_.notify_one();
	if(thread_.joinable()) thread_.join();
}


void scene_point_cloud::loader::thread_main_() {
	while(! should_exit_) {
		thread_iteration_();
		std::this_thread::sleep_for( loader_period_ );
	}
}


void scene_point_cloud::loader::thread_iteration_() {
	// Wait until requested to wake up
	std::unique_lock<std::mutex> lk(wake_up_mut_);
	wake_up_cv_.wait(lk, [this]{ return should_wake_up_ || should_exit_; });
	if(should_exit_) return;
	
	should_wake_up_ = false;
	running_ = true;
	
	// Read request
	request* req = next_request_.exchange(nullptr);

	if(req) {	
		// Process request using POV point cloud
		std::size_t sz = point_cloud_.extract(
			req->buffer,
			req->buffer_capacity,
			req->fr
		);
		
		// Make response available
		response_.size = sz;
		response_available_ = true;
		
		delete req;
	}
	
	running_ = false;
}


void scene_point_cloud::loader::set_next_request(const request& req) {
	request* old = next_request_.exchange(new request(req));
	if(old) delete old;
	
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
	
	rsp = response_;
	response_available_ = false;
	return true;
}



/////////////////////////////


scene_object_shader_program* scene_point_cloud::shader_program_ = nullptr;
const GLsizei scene_point_cloud::default_point_buffer_capacity_ = 1024 * 1024;


scene_point_cloud::scene_point_cloud(const scene& sc, const point_cloud_full& pc, GLsizei cap) :
	scene_object(sc, pc),
	point_buffer_capacity_(cap),
	pov_point_cloud_(pc)
{
	setup_loader_();
}


scene_point_cloud::scene_point_cloud(const scene& sc, const point_cloud_xyz& pc, const rgb_color& col, GLsizei cap) :
	scene_object(sc, pc),
	point_buffer_capacity_(cap),
	pov_point_cloud_(pc, col)
{
	set_unique_color(pov_point_cloud_.begin(), pov_point_cloud_.end(), col);
	setup_loader_();
}


scene_point_cloud::~scene_point_cloud() {
	delete loader_;
}



void scene_point_cloud::setup_loader_() {
	loader_ = new loader(pov_point_cloud_);
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
			std::swap(loader_point_buffer_, renderer_point_buffer_);
			renderer_point_buffer_size_ = rsp.size;
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
	if(! shader_program_)
		shader_program_ = new scene_object_shader_program(
			std::string((const char*)shaders_point_cloud_vert, shaders_point_cloud_vert_len),
			std::string((const char*)shaders_point_cloud_frag, shaders_point_cloud_frag_len)
		);

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
	
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Winvalid-offsetof"
	std::ptrdiff_t color_offset = offsetof(point_full, color);
	// unsafe use of offsetof, but works
	#pragma GCC diagnostic pop
	
	glBindVertexArray(vertex_array_object_);
	glBindBuffer(GL_ARRAY_BUFFER, renderer_point_buffer_);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, stride, 0);
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, stride, reinterpret_cast<const GLvoid*>(color_offset));
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


void scene_point_cloud::gl_uninitialize_() {
	glBindBuffer(GL_ARRAY_BUFFER, loader_point_buffer_);
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDeleteBuffers(1, &loader_point_buffer_);
	glDeleteBuffers(1, &renderer_point_buffer_);
	
	glDeleteVertexArrays(1, &vertex_array_object_);
}


void scene_point_cloud::gl_draw_() {
	// If new data set is available from loader, take it now
	take_loader_reponse_();

	shader_program_->use();
	shader_program_->mvp_matrix = mvp_matrix_;
		
	glBindVertexArray(vertex_array_object_);
	glDrawArrays(GL_POINTS, 0, renderer_point_buffer_size_);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void scene_point_cloud::mvp_was_updated_() {
	if(! initialized()) return;

	// Send new request for this camera position to loader,
	// but only if there is no response waiting to be accepted (in gl_draw)
	if(! loader_->response_available()) {
		frustum fr = scene_.get_camera().viewing_frustum();
		fr = fr.transform( object_.absolute_pose().view_transformation_inverse() );
		loader::request req {
			fr,
			loader_point_buffer_mapping_,
			(std::size_t)point_buffer_capacity_
		};
		loader_->set_next_request(req);
	}
}


}
