#include "octree_traits.h"

namespace pcf {

template class tree_point_cloud<octree_traits, point_xyz>;
template class tree_point_cloud<octree_traits, point_full>;

}