#ifndef PCFVW_SCENE_OBJECT_H_
#define PCFVW_SCENE_OBJECT_H_

#include "../gl_object.h"

namespace pcf {

class scene_object : public gl_object {
private:
	bool visible_ = true;
	
public:
	virtual ~scene_object() { }
	
	bool initialized() const { return initialized_; }
	
	virtual void initialize() const { }
	virtual void draw() const = 0;
};

}

#endif