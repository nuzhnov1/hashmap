// hash_functions.hpp

#ifndef _HASH_FUNCTIONS_
#define _HASH_FUNCTIONS_


#include <cstddef>


// Functions which translate hash_val to index in hash table
// This functions has two parameters:
// 		1) hash_val - value after hashing key
//		2) buckets_count - count of buckets in hash table

// Multiplicative hash function
size_t mul_hash(size_t hash_val, size_t buckets_count);
// Modular hash function
size_t mod_hash(size_t hash_val, size_t buckets_count);


// Probing functions for open addressing hash tables.
// This functions has following parameters:
//		1) hash_val - result of functions "mul_hash" or "mod_hash"
//		2) index - number of probe
//		3) buckets_count - count of buckets in hash table

// Linear probing function
size_t linear_probing(size_t hash_val, size_t index, size_t buckets_count);
// Quadratic probing function
size_t quadratic_probing(size_t hash_val, size_t index, size_t buckets_count);
// Double probing
// other_hash_value - is a hash value obtained by another hashing function
size_t double_probing(size_t hash_val, size_t other_hash_val,
						size_t index, size_t buckets_count);


#endif  // _HASH_FUNCTIONS_
