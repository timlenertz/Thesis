#include "gl_object.h"
#include <stdexcept>

namespace pcf {

void gl_object::initialize() {
	if(initialized_) throw std::logic_error("GL object was already initialized.");
	
	this->gl_initialize_();
	initialized_ = true;	
}

void gl_object::uninitialize() {
	if(! initialized_) throw std::logic_error("GL object was not initialized.");
	
	this->gl_uninitialize_();
	initialized_ = false;	
}

void gl_object::draw() {
	if(! initialized_) initialize();	
	this->gl_draw_();
}


}