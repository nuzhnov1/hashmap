#include <hash/hash_functions.hpp>


size_t mul_hash(size_t hash_val, size_t table_size)
{
	// The golden ratio
	constexpr auto PHI = 0.61803398874989484820;
	double frac = hash_val * PHI - static_cast<size_t>(hash_val * PHI);

	return static_cast<size_t>(table_size * frac);
}

size_t mod_hash(size_t hash_val, size_t table_size)
{
	return static_cast<size_t>(hash_val % table_size);
}


size_t linear_probing(size_t hash_val, size_t index, size_t table_size)
{
	return (hash_val + index) % table_size;
}

size_t quadratic_probing(size_t hash_val, size_t index, size_t table_size)
{
	return (hash_val + index * index) % table_size;
}

size_t double_probing(size_t hash_val, size_t other_hash_val,
						size_t index, size_t table_size)
{
	return (hash_val + other_hash_val * index) % table_size;
}
