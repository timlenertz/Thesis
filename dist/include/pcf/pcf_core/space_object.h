#ifndef PCF_SPACE_OBJECT_H_
#define PCF_SPACE_OBJECT_H_

#include "geometry/pose.h"
#include "geometry/angle.h"
#include "space_object_fwd.h"
#include <Eigen/Geometry>
#include <set>
#include <string>

namespace pcf {

struct bounding_box;

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
	
	void transform_(const Eigen::Affine3f&);
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
	
	virtual ~space_object();

	bool has_parent_space_object() const;
	const space_object& parent_space_object() const;
	space_object& parent_space_object();
	
	const pose& relative_pose() const;
	pose absolute_pose() const;
	
	void set_relative_pose(const pose&);
	
	void set_parent(space_object&, const pose& new_relative_pose = pose());
	void set_no_parent(const pose& new_pose = pose());
	
	const std::string& get_name() const { return name_; }
	void set_name(const std::string& nm) { name_ = nm; }
	
	void handle_update();
		
	template<typename Transformation>
	void transform(const Transformation& t) {
		transform_(Eigen::Affine3f(t));
	}
	
	void move(const Eigen::Vector3f& t) { transform(Eigen::Translation3f(t)); }
	void move(float x, float y, float z) { move(Eigen::Vector3f(x, y, z)); }
	void move_x(float x) { move(x, 0, 0); }
	void move_y(float y) { move(0, y, 0); }
	void move_z(float z) { move(0, 0, z); }
	
	void rotate_x_axis(angle a) { transform(Eigen::AngleAxisf(a, Eigen::Vector3f::UnitX())); }
	void rotate_y_axis(angle a) { transform(Eigen::AngleAxisf(a, Eigen::Vector3f::UnitY())); }
	void rotate_z_axis(angle a) { transform(Eigen::AngleAxisf(a, Eigen::Vector3f::UnitZ())); }
	
	/**
	Get bounding box enclosing the space object.
	Throws exception if not implemented by subclass.
	*/
	virtual bounding_box box() const;
	
	/**
	Get space bounding box enclosing the space object.
	The returned space object is a child of this space object, and thus will have the same absolute pose.
	*/
	space_bounding_box space_box();
};


}

#endif