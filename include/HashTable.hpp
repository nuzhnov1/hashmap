#ifndef _HASHTABLE_
#define _HASHTABLE_


#include <iostream>
#include <ostream>
#include <vector>
#include <forward_list>
#include <initializer_list>
#include <functional>
#include <utility>
#include <stdexcept>
#include <memory>
#include <cmath>
#include <cstddef>

#include <hash/hash.hpp>
#include <hash/hash_functions.hpp>


// Hash table with collision chains
template
<
    class _Key, class _Data,
    class _Hasher = hash<_Key>,
    class _KeyEqual = std::equal_to<_Key>,
    class _Allocator = std::allocator<std::pair<const _Key, _Data>>
>
class HashTable
{
public:
    using _key_t         = _Key;
    using _mapped_t      = _Data;
    using _value_t       = std::pair<const _key_t, _mapped_t>;
    using _hasher_t      = _Hasher;
    using _key_equal_t   = _KeyEqual;
    using _allocator_t   = _Allocator;

private:
    using _bucket_t = std::forward_list<_value_t, _allocator_t>;
    using _table_iterator        
        = typename std::vector<_bucket_t>::iterator;
    using _const_table_iterator  
        = typename std::vector<_bucket_t>::const_iterator;

public:
    class iterator;
    class const_iterator;
    using bucket_iterator       = typename _bucket_t::iterator;
    using const_bucket_iterator = typename _bucket_t::const_iterator;

private:
    static constexpr size_t DEFAULT_COUNT_BUCKETS  = 16;
    static constexpr float DEFAULT_MAX_LOAD_FACTOR = 1.0f;
    
    std::vector<_bucket_t> _m_buckets;  // Collision chains
    size_t _m_count;                    // Count of items in table
    float _m_max_load_factor;           // Max load factor
    _hasher_t _m_hasher;                // Hasher functor
    _key_equal_t _m_key_equal;          // Key equal functor
    _allocator_t _m_allocator;          // Allocator for _value_t

    // Returns the load factor of the table if it had specified count elements
    float _load_factor(size_t _count) const noexcept
    { return _count / _m_buckets.size(); }

public:
    // Constructors and destructor
    ///////////////////////////////////////////////////////////////////////////

    // Default constructor with optional parameters
    explicit HashTable
    (
        size_t _count_buckets = DEFAULT_COUNT_BUCKETS,
        const _hasher_t& _hasher = _hasher_t(),
        const _key_equal_t& _key_equal = _key_equal_t(),
        const _allocator_t& _allocator = _allocator_t()
    ):
        _m_buckets{_count_buckets},
        _m_count{0},
        _m_max_load_factor{DEFAULT_MAX_LOAD_FACTOR},
        _m_hasher{_hasher},
        _m_key_equal{_key_equal},
        _m_allocator{_allocator}
    {}

    // Constructor with the allocator parameter
    explicit HashTable(const _allocator_t& _alloc):
        _m_buckets{DEFAULT_COUNT_BUCKETS},
        _m_count{0},
        _m_max_load_factor{DEFAULT_MAX_LOAD_FACTOR},
        _m_hasher{_hasher_t()},
        _m_key_equal{_key_equal_t()},
        _m_allocator{_allocator_t()}
    {}

    // Range-based constructor
    template <class InputIterator>
    explicit HashTable
    (
        const InputIterator& begin, const InputIterator& end,
        size_t _count_buckets = DEFAULT_COUNT_BUCKETS,
        const _hasher_t& _hasher = _hasher_t(),
        const _key_equal_t& _key_equal = _key_equal_t(),
        const _allocator_t& _allocator = _allocator_t()
    ):
        _m_buckets{_count_buckets},
        _m_count{0},
        _m_max_load_factor{DEFAULT_MAX_LOAD_FACTOR},
        _m_hasher{_hasher},
        _m_key_equal{_key_equal},
        _m_allocator{_allocator}
    {
        insert(begin, end);
    }

    // Copy constructor
    HashTable(const HashTable& _other):
        _m_buckets{_other._m_buckets},
        _m_count{_other._m_count},
        _m_max_load_factor{_other._m_max_load_factor},
        _m_hasher{_other._m_hasher},
        _m_key_equal{_other._m_key_equal},
        _m_allocator{_other._m_allocator}
    {}

    // Copy constuctor with allocator parameter
    HashTable(const HashTable& _other, const _allocator_t& _alloc):
        _m_buckets{_other._m_buckets},
        _m_count{_other._m_count},
        _m_max_load_factor{_other._m_max_load_factor},
        _m_hasher{_other._m_hasher},
        _m_key_equal{_other._m_key_equal},
        _m_allocator{_alloc}
    {}

    // Move constructor
    HashTable(HashTable&& _other):
        _m_buckets{std::move(_other._m_buckets)},
        _m_count{_other._m_count},
        _m_max_load_factor{_other._m_max_load_factor},
        _m_hasher{std::move(_other._m_hasher)},
        _m_key_equal{std::move(_other._m_key_equal)},
        _m_allocator{std::move(_other._m_allocator)}
    {}

    // Move constuctor with allocator parameter
    HashTable(HashTable&& _other, const _allocator_t& _alloc):
        _m_buckets{std::move(_other._m_buckets)},
        _m_count{_other._m_count},
        _m_max_load_factor{_other._m_max_load_factor},
        _m_hasher{std::move(_other._m_hasher)},
        _m_key_equal{std::move(_other._m_key_equal)},
        _m_allocator{std::move(_alloc)}
    {}

    // Constructor based on the initialization list
    HashTable
    (
        std::initializer_list<_value_t> _il,
        size_t _count_buckets = DEFAULT_COUNT_BUCKETS,
        const _hasher_t& _hasher = _hasher_t(),
        const _key_equal_t& _key_equal = _key_equal_t(),
        const _allocator_t& _allocator = _allocator_t()
    ):
        _m_buckets{_count_buckets},
        _m_count{0},
        _m_max_load_factor{DEFAULT_MAX_LOAD_FACTOR},
        _m_hasher{_hasher},
        _m_key_equal{_key_equal},
        _m_allocator{_allocator}
    {
        insert(_il);
    }

    // Destructor
    ~HashTable()
    {}

    ///////////////////////////////////////////////////////////////////////////


    // Assigment operator
    ///////////////////////////////////////////////////////////////////////////

    // Assignment by copying
    HashTable& operator=(const HashTable& _other)
    {
        _m_buckets = _other._m_buckets;
        _m_count = _other._m_count;
        _m_max_load_factor = _other._m_max_load_factor;
        _m_hasher = _other._m_hasher;
        _m_key_equal = _other._m_key_equal;
        _m_allocator = _other._m_allocator;

        return *this;
    }

    // Assignment by moving
    HashTable& operator=(HashTable&& _other)
    {
        _m_buckets = std::move(_other._m_buckets);
        _m_count = _other._m_count;
        _m_max_load_factor = _other._m_max_load_factor;
        _m_hasher = std::move(_other._m_hasher);
        _m_key_equal = std::move(_other._m_key_equal);
        _m_allocator = std::move(_other._m_allocator);

        return *this;
    }

    // Assignment based on the initialization list
    HashTable& operator=(std::initializer_list<_value_t> _il)
    {
        clear();
        insert(_il);

        return *this;
    }

    ///////////////////////////////////////////////////////////////////////////


    // Iterators
    ///////////////////////////////////////////////////////////////////////////

    // Returns the iterator set to the beginning of the table
    iterator begin() noexcept
    { return iterator(*this, _m_buckets.begin()); }
    // Returns the const iterator set to the beginning of the table
    const_iterator begin() const noexcept
    { return const_iterator(*this, _m_buckets.cbegin()); }
    // Returns the const iterator set to the beginning of the table
    const_iterator cbegin() const noexcept
    { return const_iterator(*this, _m_buckets.cbegin()); }
    // Returns the iterator set to the end of the table
    iterator end() noexcept
    { return iterator(*this); }
    // Returns the const iterator set to the end of the table
    const_iterator end() const noexcept
    { return const_iterator(*this); }
    // Returns the const iterator set to the end of the table
    const_iterator cend() const noexcept
    { return const_iterator(*this); }

    ///////////////////////////////////////////////////////////////////////////


    // Capacity and size
    ///////////////////////////////////////////////////////////////////////////

    // Count of items in table
    size_t size() const noexcept { return _m_count; }
    // Checking the table for emptiness
    bool empty() const noexcept {return _m_count == 0; }

    ///////////////////////////////////////////////////////////////////////////


    // Elements access
    ///////////////////////////////////////////////////////////////////////////

    // Indexing operator

    _mapped_t& operator[](const _key_t& _key)
    {
        iterator iter = find(_key);

        // If an element with such a key was founded
        if (iter != end())
            return iter->second;
        
        // Otherwise, add it to containter

        // If an overflow of the table occurs after the addition,
        // then it must first be expanded
        if (_load_factor(_m_count + 1) > max_load_factor)
            reverse(_m_count + 1);

        _table_iterator table_iter = iter._m_table_iter;
        table_iter->push_front(std::make_pair(_key, _mapped_t{}));
        _m_count++;
    }

    _mapped_t& operator[](_key_t&& _key)
    {
        iterator iter = find(_key);

        // If an element with such a key was founded, then return it
        if (iter != end())
            return iter->second;
        
        // Otherwise, add it to containter

        // If an overflow of the table occurs after the addition,
        // then it must first be expanded
        if (_load_factor(_m_count + 1) > max_load_factor)
            reverse(_m_count + 1);

        _table_iterator table_iter = iter._m_table_iter;
        table_iter->push_front(std::make_pair(_key, _mapped_t{}));
        _m_count++;
    }

    // Access to the element by key, if the element is not found,
    // an out_of_range exception is thrown
    
    _mapped_t& at(const _key_t& _key)
    {
        iterator iter = find(_key);

        if (iter != end())
            return iter->second;
        else
            throw std::out_of_range("the element with this key was not found");
    }

    const _mapped_t& at(const _key_t& _key) const
    {
        const_iterator iter = find(_key);

        if (iter != end())
            return iter->second;
        else
            throw std::out_of_range("the element with this key was not found");
    }

    // Accessing an element by key and returning an iterator

    iterator find(const _key_t& _key)
    {
        size_t i = bucket(_key);
        _table_iterator table_iter = _m_buckets.begin() + i;

        for
        (
            bucket_iterator buck_iter = _m_buckets[i].begin();
            buck_iter != _m_buckets[i].end();
            buck_iter++
        )
            if (_m_key_equal(_key, buck_iter->first))
                return iterator(*this, table_iter, buck_iter);
        
        return iterator(*this);
    }

    const_iterator find(const _key_t& _key) const
    {
        size_t i = bucket(_key);
        _const_table_iterator table_iter = _m_buckets.cbegin() + i;

        for
        (
            const_bucket_iterator buck_iter = _m_buckets[i].cbegin();
            buck_iter != _m_buckets[i].cend();
            buck_iter++
        )
            if (_m_key_equal(_key, buck_iter->first))
                return const_iterator(*this, table_iter, buck_iter);
        
        return const_iterator(*this);
    }

    // Returns count of items with specified key in table
    // (1 if there is such an element, 0 otherwise)
    size_t count(const _key_t& _key) const
    { return find(_key) != cend(); }

    ///////////////////////////////////////////////////////////////////////////


    // Modifiers
    ///////////////////////////////////////////////////////////////////////////

    // Insert operations

    // Inserting a single element by copying
    std::pair<iterator, bool> insert(const _value_t& _val)
    {
        iterator iter = find(_val.first);
        _table_iterator table_iter = iter._m_table_iter;

        // If an element with such a key was founded
        if (iter != end())
            return std::make_pair(iter, false);
        
        // Otherwise, add it to containter

        // If an overflow of the table occurs after the addition,
        // then it must first be expanded
        if (_load_factor(_m_count + 1) > max_load_factor)
            reverse(_m_count + 1);

        table_iter->push_front(_val);
        _m_count++;
    }

    // Inserting a single element by moving
    std::pair<iterator, bool> insert(_value_t&& _val)
    {
        iterator iter = find(_val.first);
        _table_iterator table_iter = iter._m_table_iter;

        // If an element with such a key was founded
        if (iter != end())
            return std::make_pair(iter, false);
        
        // Otherwise, add it to containter

        // If an overflow of the table occurs after the addition,
        // then it must first be expanded
        if (_load_factor(_m_count + 1) > max_load_factor)
            reverse(_m_count + 1);

        table_iter->push_front(_val);
        _m_count++;
    }

    // Inserting a range of values
    template <class InputIterator>
    void insert(InputIterator _first, InputIterator _last)
    {
        for (InputIterator iter = _first; iter < _last; iter++)
            if (!insert(*iter).second)
                throw std::exception("failed to insert range of values");
    }

    // Inserting an initialization list
    void insert(std::initializer_list<_value_t> _il)
    {
        for (auto iter = _il.begin(); iter < _il.end(); iter++)
            if (!insert(*iter).second)
                throw std::exception("failed to insert initializer list");
    }

    // Erase operations

    // Erase item from table by specified key
    void erase(const _key_t& _key)
    {
        iterator iter = find(_key);
        _table_iterator table_iter = iter._m_table_iter;
        
        table_iter->remove_if
        (
            [&](const _value_t& val) mutable
            {
                _m_count--;
                return _m_key_equal(val.first, _key);
            }
        );
    }

    // Clear the table
    void clear() noexcept
    {
        _m_buckets.clear();
        _m_count = 0;
    }

    ///////////////////////////////////////////////////////////////////////////


    // Bucket interface
    ///////////////////////////////////////////////////////////////////////////

    // Returns the iterator set to the begining of the specified bucket
    bucket_iterator begin(size_t _n) noexcept
    { return _m_buckets[_n].begin(); }
    // Returns the const iterator set to the begining of the specified bucket
    const_iterator begin(size_t _n) const noexcept
    { return _m_buckets[_n].cbegin(); }
    // Returns the const iterator set to the begining of the specified bucket
    const_iterator cbegin(size_t _n) const noexcept
    { return _m_buckets[_n].cbegin(); }
    // Returns the iterator set to the end of the specified bucket
    iterator end(size_t _n) noexcept
    { return _m_buckets[_n].end(); }
    // Returns the const iterator set to the end of the specified bucket
    const_iterator end(size_t _n) const noexcept
    { return _m_buckets[_n].cend(); }
    // Returns the const iterator set to the end of the specified bucket
    const_iterator cend(size_t _n) const noexcept
    { return _m_buckets[_n].cend(); }

    // Returns count of buckets in table
    size_t buckets_count() const noexcept { return _m_buckets.size(); }
    // Returns size of specified bucket
    size_t bucket_size(size_t _n) const noexcept
    { return std::distance(_m_buckets[_n].cbegin(), _m_buckets[_n].cend()); }
    // Returns number of bucket by specified key
    size_t bucket(const _key_t& _key) const noexcept
    { return mul_hash(_m_hasher(_key), _m_buckets.size()); }

    ///////////////////////////////////////////////////////////////////////////


    // Hash policy
    ///////////////////////////////////////////////////////////////////////////
    
    // Returns the average number of elements per bucket
    float load_factor() const noexcept
    { return _m_count / _m_buckets.size(); }

    // Returns current maximum load factor
    float max_load_factor() const noexcept
    { return _m_max_load_factor; }

    // Set the maximum load factor to specified value
    void max_load_factor(float _ml) const noexcept
    { _m_max_load_factor = _ml; }

    // Sets the number of buckets to count and rehashes the container
    void rehash(size_t _count_buckets)
    {
        // If the new number of buckets makes load factor more than maximum
        // load factor... 
        if (_m_max_load_factor < (_m_count / _count_buckets))
            // then the new number of buckets is at least:
            _count_buckets = _m_count / _m_max_load_factor;
        
        std::vector<_bucket_t> new_buckets(_count_buckets);

        for (_value_t& value : *this)
        {
            size_t i = mul_hash(_m_hasher(value.first), _count_buckets);
            new_buckets[i].push_front(std::move(value));
        }

        _m_buckets = std::move(new_buckets);
    }

    // Sets the number of buckets to the number needed to accomodate at
    // least count elements without exceeding maximum load factor and
    // rehashes the container
    void reverse(size_t _count)
    { rehash(std::ceil((float)_count / _m_max_load_factor)); }

    ///////////////////////////////////////////////////////////////////////////


    // Observers
    ///////////////////////////////////////////////////////////////////////////

    // Returns the function used to hash the keys
    _hasher_t hash_function() const noexcept
    { return _m_hasher; }

    // Returns the function used to compare keys for equality
    _key_equal_t key_eq() const noexcept
    { return _m_key_equal; }

    // Returns the using allocator
    _allocator_t get_allocator() const noexcept
    { return _m_allocator; }

    ///////////////////////////////////////////////////////////////////////////


    // Print operation
    ///////////////////////////////////////////////////////////////////////////
    std::ostream& operator<<(std::ostream& _stream)
    {
        size_t i = 0;
        for (const auto& table_iter : _m_buckets)
        {
            _stream << "[" << i << "]: ";

            size_t j = 0;
            for (const auto& bucket_iter : table_iter)
            {
                _stream << "-> " << bucket_iter.fisrt <<
                    "[" << bucket_iter.second << "] ";
                j++;
            }
            _stream << std::endl;
            
            i++;
        }
        _stream << std::endl;

        return _stream;
    }
    ///////////////////////////////////////////////////////////////////////////

    // Iterator
    class iterator:
        public std::iterator<std::forward_iterator_tag, _value_t>
    {
    private:
        HashTable* _m_ht_ptr;
        _table_iterator _m_table_iter;
        bucket_iterator _m_buck_iter;

        // Default constructor
        iterator(HashTable& _table):
            _m_ht_ptr{&_table},
            _m_table_iter{},
            _m_buck_iter{}
        {}

        // Constructor with table iterator parameter
        iterator(HashTable& _table, _table_iterator _iter):
            _m_ht_ptr{&_table},
            _m_table_iter{_iter}
        {
            if (_m_table_iter == _m_ht_ptr->end())
                return;
            
            _m_buck_iter = _m_table_iter->begin();

            while (_m_buck_iter == _m_table_iter->end())
            {
                _m_table_iter++;
                
                if (_m_table_iter != _m_ht_ptr->end())
                    _m_buck_iter = _m_table_iter->begin();
                else
                    break;
            }
        }

        // Constructor with table iterator and bucket iterator parameters
        iterator
        (
            HashTable& _table,
            _table_iterator _table_iter, bucket_iterator _buck_iter
        ):
            _m_ht_ptr{&_table},
            _m_table_iter{_table_iter},
            _m_buck_iter{_buck_iter}
        {}
    
    public:
        // Copy constructor
        iterator(const iterator& _other) = default;
        // Move constructor
        iterator(iterator&& _other) = default;
        // Destructor
        ~iterator() {}

        // Assignment by copying
        iterator& operator=(const iterator& _other) = default;
        // Assigment by moving
        iterator& operator=(iterator&& _other) = default;

        // Equality operator
        bool operator==(const iterator& _other) const noexcept
        {
            return _m_ht_ptr == _other._m_ht_ptr &&
                   _m_table_iter == _other._m_table_iter &&
                   _m_buck_iter == _other._m_buck_iter;
        }

        // Inequality operator
        bool operator!=(const iterator& _other) const noexcept
        { return !(*this == _other); }

        // Dereference Operator
        _value_t& operator*() const noexcept
        { return *_m_buck_iter; }

        // Prefix increment operator
        iterator& operator++()
        {
            if (_m_table_iter == _m_ht_ptr->end())
                return *this;

            _m_buck_iter++;

            while (_m_buck_iter == _m_table_iter->end())
            {
                _m_table_iter++;
                
                if (_m_table_iter != _m_ht_ptr->end())
                    _m_buck_iter = _m_table_iter->begin();
                else
                    break;
            }

            return *this;
        }

        // Postfix increment operator
        iterator& operator++(int)
        {
            iterator temp = *this;
            ++(*this);

            return temp;
        }

    };
    ///////////////////////////////////////////////////////////////////////////

    // Const iterator
    class const_iterator:
        public std::iterator<std::forward_iterator_tag, _value_t>
    {
    private:
        const HashTable* _m_ht_ptr;
        _const_table_iterator _m_table_iter;
        const_bucket_iterator _m_buck_iter;

        // Default constructor
        const_iterator(HashTable& _table):
            _m_ht_ptr{&_table},
            _m_table_iter{},
            _m_buck_iter{}
        {}

        // Constructor with table iterator parameter
        const_iterator(HashTable& _table, _table_iterator _iter):
            _m_ht_ptr{&_table},
            _m_table_iter{_iter}
        {
            if (_m_table_iter == _m_ht_ptr->cend())
                return;
            
            _m_buck_iter = _m_table_iter->cbegin();

            while (_m_buck_iter == _m_table_iter->cend())
            {
                _m_table_iter++;
                
                if (_m_table_iter != _m_ht_ptr->cend())
                    _m_buck_iter = _m_table_iter->cbegin();
                else
                    break;
            }
        }

        // Constructor with table iterator and bucket iterator parameters
        const_iterator
        (
            HashTable& _table,
            _table_iterator _table_iter, bucket_iterator _buck_iter
        ):
            _m_ht_ptr{&_table},
            _m_table_iter{_table_iter},
            _m_buck_iter{_buck_iter}
        {}
    
    public:
        // Copy constructor
        const_iterator(const const_iterator& _other) = default;
        // Move constructor
        const_iterator(const_iterator&& _other) = default;
        // Destructor
        ~const_iterator() {}

        // Assignment by copying
        const_iterator& operator=(const const_iterator& _other) = default;
        // Assigment by moving
        const_iterator& operator=(const_iterator&& _other) = default;

        // Equality operator
        bool operator==(const const_iterator& _other) const noexcept
        {
            return _m_ht_ptr == _other._m_ht_ptr &&
                   _m_table_iter == _other._m_table_iter &&
                   _m_buck_iter == _other._m_buck_iter;
        }

        // Inequality operator
        bool operator!=(const const_iterator& _other) const noexcept
        { return !(*this == _other); }

        // Dereference Operator
        const _value_t& operator*() const noexcept
        { return *_m_buck_iter; }

        // Prefix increment operator
        const_iterator& operator++()
        {
            if (_m_table_iter == _m_ht_ptr->cend())
                return *this;

            _m_buck_iter++;

            while (_m_buck_iter == _m_table_iter->cend())
            {
                _m_table_iter++;
                
                if (_m_table_iter != _m_ht_ptr->cend())
                    _m_buck_iter = _m_table_iter->cbegin();
                else
                    break;
            }

            return *this;
        }

        // Postfix increment operator
        const_iterator& operator++(int)
        {
            const_iterator temp = *this;
            ++(*this);

            return temp;
        }

    };
    ///////////////////////////////////////////////////////////////////////////

}; // HashTable


#endif  // _HASHTABLE_
