#ifndef PCFVW_SCENE_H_
#define PCFVW_SCENE_H_

#include <set>
#include <unique_ptr>
#include "gl_object.h"

namespace pcf {

class scene_object;
class scene_point_cloud;

class scene : public gl_object {
private:
	std::set<std::unique_ptr<scene_object>> objects_;

protected:
	void gl_draw_() override;

public:
	~scene();
		
	template<typename Cloud>
	scene_point_cloud& add_point_cloud(Cloud&&);
};

}

#endif