namespace pcf {


template<typename T>
const std::size_t default_allocator<T>::mmap_threshold_ = 1024 * 1024 * 1024;


template<typename T>
T* default_allocator<T>::allocate(std::size_t n, const T* hint) {
	if(n*sizeof(T) >= mmap_threshold_) return mmap_allocator_.allocate(n, hint);
	else return standard_allocator_.allocate(n, hint);
}


template<typename T>
void default_allocator<T>::deallocate(T* buf, std::size_t n) {
	if(n*sizeof(T) >= mmap_threshold_) mmap_allocator_.deallocate(buf, n);
	else standard_allocator_.deallocate(buf, n);
}


}