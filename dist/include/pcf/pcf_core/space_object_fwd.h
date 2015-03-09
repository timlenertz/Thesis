#ifndef PCF_SPACE_OBJECT_FWD_H_
#define PCF_SPACE_OBJECT_FWD_H_

namespace pcf {

struct bounding_box;
struct plane;

class space_object;
class space_object_observer;
template<typename> class space_object_wrapper;

using space_bounding_box = space_object_wrapper<bounding_box>;
using space_plane = space_object_wrapper<plane>;

}

#endif
