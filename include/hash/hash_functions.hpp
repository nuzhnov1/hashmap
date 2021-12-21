// hash_functions.hpp

#ifndef _HASH_FUNCTIONS_
#define _HASH_FUNCTIONS_


#include <cstddef>


// Functions which translate hash_val to index in hash table
// This functions has two parameters:
// 		1) hash_val - value after hashing key
//		2) table_size - size of hash table

// Multiplicative hash function
size_t __mul_hash(size_t hash_val, size_t table_size);
// Modular hash function
size_t __mod_hash(size_t hash_val, size_t table_size);


// Probing functions for open addressing hash tables.
// This functions has following parameters:
//		1) hash_val - result of functions "__mul_hash" or "__mod_hash"
//		2) index - number of probe
//		3) table_size - size of hash table

// Linear probing function
size_t __linear_probing(size_t hash_val, size_t index, size_t table_size);
// Quadratic probing function
size_t __quadratic_probing(size_t hash_val, size_t index, size_t table_size);
// Double probing
// other_hash_value - is a hash value obtained by another hashing function
size_t __double_probing(size_t hash_val, size_t other_hash_val,
						size_t index, size_t table_size);


#endif
