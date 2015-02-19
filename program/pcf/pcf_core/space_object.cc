#include "space_object.h"
#include <stdexcept>

namespace pcf {

space_object::space_object(const pose& ps, space_object& par) :
parent_(&par), pose_(ps) {
	attach_to_parent_();
}


space_object::space_object(const pose& ps) :
parent_(nullptr), pose_(ps) { }


space_object::space_object(const space_object& obj) :
parent_(obj.parent_), pose_(obj.pose_) {
	attach_to_parent_();
}


space_object::space_object(space_object&& obj) :
parent_(obj.parent_), pose_(obj.pose_) {
	attach_to_parent_();
}

space_object::~space_object() {
	detach_from_parent_();
	detach_from_children_();
	for(auto observer : observers_) observer->handle_object_deleted();
}


void space_object::detach_from_parent_() {
	if(parent_) parent_->children_.erase(this);
}

void space_object::detach_from_children_() {
	for(auto child : children_)
		if(child->parent_ == this) child->parent_ = nullptr;
}


void space_object::attach_to_parent_() {
	if(parent_) parent_->children_.insert(this);
}


void space_object::pose_was_updated_() {
	// Can be implemented by subclass to respond to pose change.
	return;
}


void space_object::recursive_notify_pose_update_() {
	this->pose_was_updated_();
	for(auto child : children_) child->pose_was_updated_();
	for(auto observer : observers_) observer->handle_pose_update();
}


bool space_object::has_parent_space_object() const {
	return (parent_ != nullptr);
}


const space_object& space_object::parent_space_object() const {
	if(! has_parent_space_object()) throw std::logic_error("Space object has no parent.");
	else return *parent_;
}


space_object& space_object::parent_space_object() {
	if(! has_parent_space_object()) throw std::logic_error("Space object has no parent.");
	else return *parent_;
}

const pose& space_object::relative_pose() const {
	return pose_;
}


pose space_object::absolute_pose() const {
	if(has_parent_space_object())
		return pose_.transform( parent_space_object().absolute_pose().view_transformation_inverse() );
	else
		return pose_;
}


void space_object::set_relative_pose(const pose& ps) {
	pose_ = ps;
	recursive_notify_pose_update_();
}


void space_object::set_parent(space_object& par, const pose& new_relative_pose) {
	detach_from_parent_();
	parent_ = &par;
	attach_to_parent_();
	pose_ = new_relative_pose;
	recursive_notify_pose_update_();
}


void space_object::set_no_parent(const pose& new_pose) {
	detach_from_parent_();
	pose_ = new_pose;
	recursive_notify_pose_update_();
}


void space_object::attach_observer_(space_object_observer* obs) {
	observers_.insert(obs);
}


void space_object::detach_observer_(space_object_observer* obs) {
	observers_.erase(obs);
}


void space_object::handle_update() {
	for(auto observer : observers_) observer->handle_object_update();
}

space_object_observer::space_object_observer(space_object* obj) :
object_(obj) {
	object_->attach_observer_(this);
}


space_object_observer::~space_object_observer() {
	if(object_) object_->detach_observer_(this);
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

}
