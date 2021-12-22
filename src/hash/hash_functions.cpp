// hash_functions.cpp

#include <hash/hash_functions.hpp>


size_t mul_hash(size_t hash_val, size_t buckets_count)
{
	// The golden ratio
	constexpr auto PHI = 0.61803398874989484820;
	double frac = hash_val * PHI - static_cast<unsigned int>(hash_val * PHI);

	return static_cast<size_t>(buckets_count * frac);
}

size_t mod_hash(size_t hash_val, size_t buckets_count)
{
	return static_cast<size_t>(hash_val % buckets_count);
}


size_t linear_probing(size_t hash_val, size_t index, size_t buckets_count)
{
	return (hash_val + index) % buckets_count;
}

size_t quadratic_probing(size_t hash_val, size_t index, size_t buckets_count)
{
	return (hash_val + index * index) % buckets_count;
}

size_t double_probing(size_t hash_val, size_t other_hash_val,
						size_t index, size_t buckets_count)
{
	return (hash_val + other_hash_val * index) % buckets_count;
}
