#ifndef PCF_SPACE_OBJECT_H_
#define PCF_SPACE_OBJECT_H_

#include "geometry/pose.h"
#include <set>
#include <string>

namespace pcf {

class space_object_observer;

/**
Base class for object that has a pose in space relative to a coordinate system.
Can have a parent space object, meaning its pose will be defined relative to that parent. No parent means pose is absolute. Parent can be reassigned. Keeps references to its children, and recursively notifies children on pose change. Space objects cannot be copy or move assigned. So the they can have const members and be (partially) immutable. Guarantees that pointers to parent or children get properly removed when either child or parent gets deleted.
*/
class space_object {
	friend class space_object_observer;
	
private:
	space_object* parent_;
	std::set<space_object*> children_;
	std::set<space_object_observer*> observers_;
	pose pose_;
	std::string name_;
	
	void recursive_notify_pose_update_();
	
	space_object& operator=(const space_object&) = delete;
	space_object& operator=(space_object&&) = delete;

	void detach_from_parent_();
	void detach_from_children_();
	void attach_to_parent_();
	
	void attach_observer_(space_object_observer*);
	void detach_observer_(space_object_observer*);

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
	
	void handle_update();
};


/**
Observer attached to space object.
Receives notifications when space object gets changed, its pose got updated, or it got deleted. Properly
handles attaching/detaching from space object, no matter if space object or the observer get deleted
first.
*/
class space_object_observer {
protected:
	space_object* object_;
	
	space_object_observer(const space_object_observer&) = delete;
	space_object_observer& operator=(const space_object_observer&) = delete;
	
	virtual void pose_was_updated_();
	virtual void object_was_updated_();
	virtual void object_was_deleted_();
	
public:
	explicit space_object_observer(space_object&);
	~space_object_observer();

	void handle_pose_update();
	void handle_object_update();
	void handle_object_deleted();
};


}

#endif