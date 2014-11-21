#ifdef _WIN32

#include "mmap_allocator.h"

#include <utility>
#include <stdexcept>
#include <map>

#include <windows.h>
#include <tchar.h>

namespace pcf {

struct mmap_allocator_base::impl {
	struct allocation {
		HANDLE temporary_file_handle;
		HANDLE file_mapping_handle;
	};

	std::map<std::uintptr_t, std::unique_ptr<allocation>> allocations;
};


std::unique_ptr<mmap_allocator_base::impl> mmap_allocator_base::impl_ = 
	std::unique_ptr<mmap_allocator_base::impl>();
	

mmap_allocator_base::impl& mmap_allocator_base::get_impl_() {
	if(! impl_) impl_.reset(new impl);
	return *impl_;
}


mmap_allocator_base::mmap_allocator_base(const std::string& name):
	name_(name) { }


std::uintptr_t mmap_allocator_base::key_for_ptr_(const void* ptr) {
	return std::uintptr_t(ptr);
}


void* mmap_allocator_base::allocate_(std::size_t length, std::size_t align, const void* hint) {	
	HANDLE temporary_file_handle = NULL, file_mapping_handle = NULL;
	
	try {
		// Generate temporary file with unique name
		std::basic_string<TCHAR> prefix = name_;
		TCHAR temporary_file_name[MAX_PATH];
	
		UINT ret1 = GetTempFileName(TEXT("."), prefix.data(), 0, temporary_file_name);
		if(ret1 == 0) throw std::runtime_error("mmap: GetTempFileName failed.");

		temporary_file_handle = CreateFile(
			(LPTSTR) temporary_file_name,
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_TEMPORARY | FILE_FLAG_DELETE_ON_CLOSE | FILE_FLAG_RANDOM_ACCESS,
			NULL
		);
		if(temporary_file_handle == INVALID_HANDLE_VALUE)
			throw std::runtime_error("mmap: CreateFile failed.");


		// Set length of temporary file
		BOOL ret2 = SetFilePointer(temporary_file_handle, length, NULL, FILE_BEGIN);
		if(ret2 == 0) throw std::runtime_error("mmap: SetFilePointerEx failed.");

		ret2 = SetEndOfFile(temporary_file_handle);
		if(ret2 == 0) throw std::runtime_error("mmap: SetEndOfFile failed.");

	
		// Create file mapping and view
		std::basic_string<TCHAR> mapping_name = std::basic_string<TCHAR>("Local\\PCF mmap ") + name_;
		file_mapping_handle = CreateFileMapping(
			temporary_file_handle,
			NULL,
			PAGE_READWRITE,
			0, 0,
			(LPCTSTR) mapping_name.data()
		);
		if(file_mapping_handle == NULL)
			throw std::runtime_error("mmap: CreateFileMapping failed."); 
	
		LPVOID ptr = MapViewOfFile(
			file_mapping_handle,
			FILE_MAP_ALL_ACCESS,
			0, 0,
			0
		);
		if(ptr == nullptr) throw std::runtime_error("mmap: MapViewOfFile failed."); 
		else if(std::uintptr_t(ptr) % align != 0) throw std::runtime_error("Memory mapped file not properly aligned."); 



		// Add to allocations list	
		std::uintptr_t key = key_for_ptr_(ptr);
		auto& impl = get_impl_();
		impl::allocation* alloc = new impl::allocation;
		alloc->temporary_file_handle = temporary_file_handle;
		alloc->file_mapping_handle = file_mapping_handle;
		impl.allocations[key].reset(alloc);
	
		return ptr;

	
	} catch(...) {
		if(temporary_file_handle) CloseHandle(temporary_file_handle);
		if(file_mapping_handle) CloseHandle(file_mapping_handle);
		throw;
	}
}



void mmap_allocator_base::deallocate_(void* ptr, std::size_t length) {
	auto& impl = get_impl_();
	std::uintptr_t key = key_for_ptr_(ptr);

	auto it = impl.allocations.find(key);
	if(it == impl.allocations.end()) return;
	if(! it->second) return;
	impl::allocation& alloc = *(it->second);
	
	UnmapViewOfFile( (LPCVOID)ptr );
	CloseHandle(alloc.file_mapping_handle);
	CloseHandle(alloc.temporary_file_handle);
		
	impl.allocations.erase(it);	
}

}

#endif