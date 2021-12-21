// hash.hpp

#ifndef _HASH_
#define _HASH_


#include <cstddef>
#include <cstring>
#include <string>
#include <type_traits>

#include <hash/hash_impl.hpp>


// TODO: add choose hash method using macro in compile time
using __hash_method = __hash_impl::FNV;


// Enumeration class of groups of hashed types by default 
enum class __hashable_types
{
	OTHER 				= 0,
	POINTERS 			= 1,
	LV_REFERENCES 		= 2,
	ENUM 				= 4,
	INTEGRAL 			= 8,
	FLOATING_POINT 		= 16,
	NULLPTR				= 32,
	VOID 				= 64
};


// Matching "_Type" with one of hashable types group
template <class _Type, __hashable_types = static_cast<__hashable_types>(
	(std::is_pointer<_Type>::value) +
	(std::is_lvalue_reference<_Type>::value << 1) +
	(std::is_enum<_Type>::value << 2) +
	(std::is_integral<_Type>::value << 3) +
	(std::is_floating_point<_Type>::value << 4) +
	(std::is_null_pointer<_Type>::value << 5) +
	(std::is_void<_Type>::value << 6))
>
struct hash;


// Hash functor for generic type
template <class _Type>
struct hash<_Type, __hashable_types::OTHER>
{
	size_t operator()(const _Type& value) const noexcept
	{ return __hash_method::hash(value); }
};

// Explicit specialization of hash functor for std::string type
template<>
struct hash<std::string, __hashable_types::OTHER>
{
	size_t operator()(const std::string& value) const noexcept
	{ 
		return __hash_method::hash(value.c_str(),
			sizeof(char) * value.size()); 
	}
};

// Explicit specialization of hash functor for pointer types
template <typename _Type>
struct hash<_Type, __hashable_types::POINTERS>
{
	size_t operator()(const _Type value) const noexcept
	{ return reinterpret_cast<size_t>(value); }
};

// Explicit specialization of hash functor for lvalue references types
template <typename _Type>
struct hash<_Type, __hashable_types::LV_REFERENCES>
{
	size_t operator()(const _Type value) const noexcept
	{ return reinterpret_cast<size_t>(&value); }
};

// Explicit specialization of hash functor for enum types
template <typename _Type>
struct hash<_Type, __hashable_types::ENUM>
{
	size_t operator()(const _Type value) const noexcept
	{ return static_cast<size_t>(value); }
};

// Explicit specialization of hash functor for integral types
template<typename _Type>
struct hash<_Type, __hashable_types::INTEGRAL>
{
	size_t operator()(const _Type value) const noexcept
	{ return static_cast<size_t>(value); }
};

// Explicit specialization of hash functor for floating point types
template<typename _Type>
struct hash<_Type, __hashable_types::FLOATING_POINT>
{
	size_t operator()(const _Type value) const noexcept
	{ return value == 0 ? 0 : __hash_method::hash(value); }
};

// Explicit specialization for nullptr type
template<>
struct hash<nullptr_t, __hashable_types::NULLPTR>
{
	size_t operator()(const nullptr_t value) const noexcept { return 0; }
};

// Explicit specialization for void type
template<typename _Type>
struct hash<_Type, __hashable_types::VOID>
{
	size_t operator()() const noexcept { return 0; }
};


#endif
