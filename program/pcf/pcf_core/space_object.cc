#include "space_object.h"
#include "space_object_observer.h"
#include "space_object_wrapper.h"
#include "geometry/bounding_box.h"
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
	for(auto child : children_) child->recursive_notify_pose_update_();
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
		return pose_.transform( parent_space_object().absolute_pose().transformation_to_world() );
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


void space_object::transform_(const Eigen::Affine3f& t) {
	pose_ = pose_.transform(t);
	recursive_notify_pose_update_();
}


bounding_box space_object::box() const {
	// Default implementation
	throw std::logic_error("Space object has not bounding box.");
}

space_bounding_box space_object::space_box() {
	bounding_box b = this->box();
	space_bounding_box sb(b);
	sb.set_parent(*this);
	return sb;
}


Eigen::Affine3f space_object::transformation_from(const space_object& obj) const {
	Eigen::Affine3f obj_to_world = obj.absolute_pose().transformation_to_world();
	Eigen::Affine3f world_to_this = absolute_pose().transformation_from_world();
	return world_to_this * obj_to_world;
}


Eigen::Affine3f space_object::transformation_to(const space_object& obj) const {
	return transformation_from(obj).inverse();
}



}
