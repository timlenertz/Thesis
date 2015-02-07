#ifndef PCFHL_SPACE_OBJECT_H_
#define PCFHL_SPACE_OBJECT_H_

#include <memory>

namespace pcf {

class space_object;
class point_cloud_full;

class SpaceObject {
protected:
	std::unique_ptr<space_object> object_;

public:
	explicit SpaceObject(space_object*);
	virtual ~SpaceObject();
	
	#ifndef SWIG
	template<class Visitor> void accept(Visitor&);
	point_cloud_full& point_cloud();
	const point_cloud_full& point_cloud() const;
	#endif
};

using SpaceObjectPtr = std::shared_ptr<SpaceObject>;

}

#include "SpaceObject.tcc"

#endif
