#ifndef PCF_SPACE_OBJECT_OBSERVER_H_
#define PCF_SPACE_OBJECT_OBSERVER_H_

namespace pcf {

class space_object;

/**
Observer attached to space object.
Receives notifications when space object gets changed, its pose got updated, or it got deleted. Properly
handles attaching/detaching from space object, no matter if space object or the observer get deleted
first.
*/
class space_object_observer {
private:
	space_object* object_;

protected:
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
	
	const space_object& observed_object() const;
	bool has_observed_object() const;
};

}

#endif