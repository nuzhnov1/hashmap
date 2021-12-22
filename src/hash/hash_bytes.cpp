// hash_bytes.cpp

#include <hash/hash_bytes.hpp>


// Implementation of FNV-1a hash algorithm for 32-bit size_t
#if __SIZEOF_SIZE_T__ == 4
size_t _Fnv_hash_bytes(const void* ptr, size_t len, size_t seed)
{
	const char* c_ptr = static_cast<const char*>(ptr);
	size_t hval = seed;

	for (; len > 0; len--, ptr++)
	{
		hval ^= static_cast<size_t>(*c_ptr);
		hval *= static_cast<size_t>(0x01000193);
	}

	return hval;
}
#elif __SIZEOF_SIZE_T__ == 8
// Implementation of FNV-1a hash algorithm for 64-bit size_t
size_t _Fnv_hash_bytes(const void* ptr, size_t len, size_t seed)
{
	const char* c_ptr = static_cast<const char*>(ptr);
	size_t hval = seed;

	for (; len > 0; len--, c_ptr++)
	{
		hval ^= static_cast<size_t>(*c_ptr);
		hval *= static_cast<size_t>(0x100000001b3);
	}

	return hval;
}


#endif
