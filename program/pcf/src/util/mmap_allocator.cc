#include "mmap_allocator.h"

#include <utility>
#include <stdexcept>
#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/filesystem.hpp>

namespace pcf {

struct mmap_allocator_base::impl {
	boost::filesystem::path path;
	boost::iostreams::mapped_file file;
};


mmap_allocator_base::mmap_allocator_base(const std::string& pathname) :
impl_(new impl) {
	if(! pathname.empty()) impl_->path = boost::filesystem::path(pathname);
	else impl_->path = boost::filesystem::unique_path();
}

mmap_allocator_base::mmap_allocator_base(mmap_allocator_base&& a) :
impl_(std::move(a.impl_)), allocated_(a.allocated_) {
	a.allocated_ = false;
}

mmap_allocator_base& mmap_allocator_base::operator=(mmap_allocator_base&& a) {
	if(allocated_) throw std::runtime_error("Cannot assign memory mapped allocator while allocated.");
	impl_ = std::move(a.impl_);
	allocated_ = a.allocated_;
	return *this;
}


mmap_allocator_base::~mmap_allocator_base() { }


void* mmap_allocator_base::allocate_(std::size_t length, std::size_t align, const void* hint) {
	if(allocated_) throw std::runtime_error("Memory mapper file allocator: cannot allocate more than once at the same time.");
	
	if(impl_->file.alignment() % align != 0) throw std::runtime_error("Memory mapped file not property aligned.");

	boost::iostreams::mapped_file_params params(impl_->path.native());
	params.flags = boost::iostreams::mapped_file::readwrite;
	params.new_file_size = length;
	params.hint = reinterpret_cast<const char*>(hint);
	
	impl_->file.open(params);
	
	allocated_ = true;
	return reinterpret_cast<void*>(impl_->file.data());
}

void mmap_allocator_base::deallocate_(void* buf, std::size_t length) {
	if(allocated_) {
		impl_->file.close();
		allocated_ = false;
	}
}

void mmap_allocator_base::remove_file_() {
	boost::filesystem::remove(impl_->path);
}


}