#include "kdtree_traits.h"

namespace pcf {

template class tree_point_cloud<kdtree_traits, point_xyz>;
template class tree_point_cloud<kdtree_traits, point_full>;

}