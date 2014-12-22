#include "space_object.h"
#include <stdexcept>

namespace pcf {

space_object::space_object(const pose& ps, const space_object& par) :
parent_(&par), pose_(ps) {
	par.children_.insert(this);
}


space_object::space_object(const pose& ps) :
parent_(nullptr), pose_(ps) { }


space_object::~space_object() {
	if(parent_) parent_->children_.erase(*this);
}


void space_object::pose_was_updated_() {
	// Can be implemented by subclass to respond to pose change.
	return;
}


void space_object::recursive_notify_pose_update_() {
	this->pose_was_updated_();
	for(auto child : children_) child->pose_was_updated_();
}


bool space_object::has_parent_space_object() const {
	return (parent_ != nullptr);
}


const space_object& space_object::parent_space_object() const {
	if(! has_parent_space_object()) throw std::logic_error("Space object has no parent, or parent was already deleted.");
	else return *parent_;
}


space_object& space_object::parent_space_object() {
	if(! has_parent_space_object()) throw std::logic_error("Space object has no parent, or parent was already deleted.");
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


}