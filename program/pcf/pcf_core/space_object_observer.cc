#include "space_object_observer.h"
#include "space_object.h"

namespace pcf {


space_object_observer::space_object_observer() { }


space_object_observer::space_object_observer(space_object& obj) {
	set_observed_object(obj);
}


space_object_observer::~space_object_observer() {
	set_no_observed_object();
}

void space_object_observer::set_observed_object(space_object& obj) {
	set_no_observed_object();
	object_ = &obj;
	object_->attach_observer_(this);
}


void space_object_observer::set_no_observed_object() {
	if(object_) {
		object_->detach_observer_(this);
		object_ = nullptr;
	}
}

	
void space_object_observer::pose_was_updated_() { 
	return;
}

void space_object_observer::object_was_updated_() {
	return;
}

void space_object_observer::object_was_deleted_() {
	return;
}


void space_object_observer::handle_pose_update() {
	this->pose_was_updated_();
}

void space_object_observer::handle_object_update() {
	this->object_was_updated_();
}

void space_object_observer::handle_object_deleted() {
	object_ = nullptr;
	this->object_was_deleted_();
}


const space_object& space_object_observer::observed_object() const {
	if(object_) return *object_;
	else throw std::logic_error("Observed object no longer exists.");
}


space_object& space_object_observer::observed_object() {
	if(object_) return *object_;
	else throw std::logic_error("Observed object no longer exists.");
}


bool space_object_observer::has_observed_object() const {
	return (object_ != nullptr);
}

}