#ifndef PCF_SPACE_OBJECT_H_
#define PCF_SPACE_OBJECT_H_

#include "geometry/pose.h"
#include <set>

namespace pcf {

/**
Base class for object that has a pose in space relative to a coordinate system.
Can have a parent space object, meaning its pose will be defined relative to that parent. No parent means pose is absolute. Parent must exist for as long as its children. Parent can be reassigned.
Keeps references to its children, and recursively notifies children on pose change. Space objects cannot be copy or move assigned. So the they can have const members and be (partially) immutable.
*/
class space_object {
private:
	space_object* parent_;
	std::set<space_object*> children_;
	pose pose_;
	
	void recursive_notify_pose_update_();
	
	space_object& operator=(const space_object&) = delete;
	space_object& operator=(space_object&&) = delete;

	void detach_from_parent_();
	void attach_to_parent_();

protected:
	/// Gets called when relative pose has changed.
	/// Either through explicit change of this object's relative pose, or through change of ancestor's relative pose.
	virtual void pose_was_updated_();

public:
	explicit space_object(const pose& = pose());
	space_object(const pose&, space_object& par);
	space_object(const space_object&);
	space_object(space_object&&);
	
	virtual ~space_object();

	bool has_parent_space_object() const;
	const space_object& parent_space_object() const;
	space_object& parent_space_object();
	
	const pose& relative_pose() const;
	pose absolute_pose() const;
	
	void set_relative_pose(const pose&);
	
	void set_parent(space_object&, const pose& new_relative_pose = pose());
	void set_no_parent(const pose& new_pose = pose());
};

}

#endif