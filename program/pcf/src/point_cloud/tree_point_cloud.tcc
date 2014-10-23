namespace pcf {

template<typename Traits, typename Point, typename Allocator> template<typename Other_cloud>
tree_point_cloud<Traits, Point, Allocator>::tree_point_cloud(Other_cloud&& pc, std::size_t leaf_cap, const Allocator& alloc) :
super(std::forward<Other_cloud>(pc), true, alloc), leaf_capacity_(leaf_cap), root_node_() {
	build_tree_();
}


template<typename Traits, typename Point, typename Allocator> template<typename Other_cloud>
tree_point_cloud<Traits, Point, Allocator>::tree_point_cloud(Other_cloud&& pc, std::size_t leaf_cap) :
super(std::forward<Other_cloud>(pc), true), leaf_capacity_(leaf_cap), root_node_(*this) {
	build_tree_();
}


template<typename Traits, typename Point, typename Allocator>
void tree_point_cloud<Traits, Point, Allocator>::build_tree_() {
	// Setup root cuboid, enclosing whole point cloud
	root_cuboid_ = Traits::root_cuboid(*this);

	// Construct tree by breath-first descent.
	// Creates node tree, and segment point cloud in-place.
	using node_with_cuboid = std::pair<
		std::reference_wrapper<node>,
		cuboid
	>;
	std::vector<node_with_cuboid> todo, next_todo; // nodes to split at current and next iteration
	todo.emplace_back(root_node_, root_cuboid_);
	
	std::size_t depth = 0;
	while(todo.size() && depth != Traits::maximal_depth) {
		// todo contains set of nodes at same level.

		// Segments for nodes in todo don't overlap
		// Can split them in simultaneously		
		#pragma omp parallel
		{
			std::vector<node_with_cuboid> next_todo_part;
		
			#pragma omp for
			for(auto it = todo.cbegin(); it < todo.cend(); ++it) {
				node& nd = it->first;
				const cuboid& cub = it->second;
								
				if(nd.seg.size() <= leaf_capacity_) continue;
		
				// Split the node
				// Rearranges data in node's segment into subsegments for children
				auto child_segments = Traits::split_node(nd.seg, cub, nd, depth);
				assert(child_segments[0].begin() == nd.seg.begin());
				assert(child_segments[Traits::number_of_children - 1].end() == nd.seg.end());

				// Create child nodes, and schedule for next iteration
				for(std::ptrdiff_t i = 0; i < Traits::number_of_children; ++i) {
					auto& seg = child_segments[i];
					if(! seg.empty()) {
						//for(Point& p : seg) mark_point(p, std::rand());
					
						nd.children[i].reset( new node(seg) );
						
						cuboid child_cub = Traits::child_cuboid(i, cub, nd, depth);
						next_todo_part.emplace_back(*nd.children[i], child_cub);
					}
				}
			}

			#pragma omp critical
			{
				next_todo.insert(next_todo.end(), next_todo_part.cbegin(), next_todo_part.cend());
			}
		}
		
		todo = next_todo;
		next_todo.clear();
		++depth;
	}
}


template<typename Traits, typename Point, typename Allocator>
bool tree_point_cloud<Traits, Point, Allocator>::verify_(const node& nd, const cuboid& cub, std::ptrdiff_t depth) const {
	for(const Point& p : nd.seg) if(! cub.contains(p)) return false;
	
	for(std::ptrdiff_t i = 0; i < Traits::number_of_children; ++i) {
		if(! nd.children[i]) continue;
		const node& c_nd = *nd.children[i];
		cuboid c_cub = Traits::child_cuboid(i, cub, nd, depth);
		bool child_ok = verify_(c_nd, c_cub, depth + 1);
		if(! child_ok) return false;
	}
	
	return true;
}


template<typename Traits, typename Point, typename Allocator>
bool tree_point_cloud<Traits, Point, Allocator>::verify() const {
	return verify_(root_node_, root_cuboid_, 0);
}


template<typename Traits, typename Point, typename Allocator>
bool tree_point_cloud<Traits, Point, Allocator>::node::is_leaf() const {
	for(const auto& c : children) if(c) return false;
	return true;
}


}