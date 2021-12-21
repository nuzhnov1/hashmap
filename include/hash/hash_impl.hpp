// hash_impl.hpp

#ifndef _HASH_IMPL_
#define _HASH_IMPL_


#include <cstddef>

#include <hash/hash_bytes.hpp>


// Implementation of some general-purpose hash methods
namespace __hash_impl
{
    // Hash functions using the FNV-1a algorithm
    struct FNV
    {
#if __SIZEOF_SIZE_T__ == 4
        constexpr static size_t INITIAL_SEED = 0x811c9dc5;
#elif __SIZEOF_SIZE_T__ == 8
        constexpr static size_t INITIAL_SEED = 0xcbf29ce484222325;
#endif

        static size_t
        hash(const void* ptr, size_t length, size_t seed = INITIAL_SEED)
        { return _Fnv_hash_bytes(ptr, length, seed); }

        template<typename _Type>
        static size_t hash(const _Type& value)
        { return hash(&value, sizeof(value)); }

        template<typename _Type>
        static size_t _hash_combine(const _Type& value, size_t hash)
        { return hash(&value, sizeof(value), hash); }
    };
}


#endif  // _HASH_IMPL_
