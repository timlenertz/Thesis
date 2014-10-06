#include "ply_writer.h"

namespace pcf {

ply_writer_base::ply_writer_base(const std::string& path) :
file_(path, std::ios_base::out | std::ios_base::trunc) {
	constexpr static std::size_t vertex_count_string_length_ = 15;

}

}