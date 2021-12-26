// HashMap.hpp

#ifndef _HASHMAP_
#define _HASHMAP_


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


// Hash map container
template
<
    class _Key, class _Data,
    class _Hasher = hash<_Key>,
    class _KeyEqual = std::equal_to<_Key>,
    class _Allocator = std::allocator<std::pair<const _Key, _Data>>
>
class HashMap
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
    static constexpr size_t MIN_COUNT_BUCKETS  = 16;
    static constexpr float DEFAULT_MAX_LOAD_FACTOR = 1.0f;
    
    std::vector<_bucket_t> _m_buckets;  // Collision chains
    size_t _m_count;                    // Count of items in map
    float _m_max_load_factor;           // Max load factor
    _hasher_t _m_hasher;                // Hasher functor
    _key_equal_t _m_key_equal;          // Key equal functor
    _allocator_t _m_allocator;          // Allocator for _value_t

    // Returns the load factor of the container
    // if it had specified count elements
    float _load_factor(size_t _count) const noexcept
    { return (float)_count / _m_buckets.size(); }

public:
    // Constructors and destructor
    ///////////////////////////////////////////////////////////////////////////

    // Default constructor with optional parameters
    explicit HashMap
    (
        size_t _count_buckets = MIN_COUNT_BUCKETS,
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
    explicit HashMap(const _allocator_t& _alloc):
        _m_buckets{MIN_COUNT_BUCKETS},
        _m_count{0},
        _m_max_load_factor{DEFAULT_MAX_LOAD_FACTOR},
        _m_hasher{_hasher_t()},
        _m_key_equal{_key_equal_t()},
        _m_allocator{_allocator_t()}
    {}

    // Range-based constructor
    template <class InputIterator>
    explicit HashMap
    (
        const InputIterator& begin, const InputIterator& end,
        size_t _count_buckets = MIN_COUNT_BUCKETS,
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
    HashMap(const HashMap& _other):
        _m_buckets{_other._m_buckets},
        _m_count{_other._m_count},
        _m_max_load_factor{_other._m_max_load_factor},
        _m_hasher{_other._m_hasher},
        _m_key_equal{_other._m_key_equal},
        _m_allocator{_other._m_allocator}
    {}

    // Copy constuctor with allocator parameter
    HashMap(const HashMap& _other, const _allocator_t& _alloc):
        _m_buckets{_other._m_buckets},
        _m_count{_other._m_count},
        _m_max_load_factor{_other._m_max_load_factor},
        _m_hasher{_other._m_hasher},
        _m_key_equal{_other._m_key_equal},
        _m_allocator{_alloc}
    {}

    // Move constructor
    HashMap(HashMap&& _other):
        _m_buckets{std::move(_other._m_buckets)},
        _m_count{_other._m_count},
        _m_max_load_factor{_other._m_max_load_factor},
        _m_hasher{std::move(_other._m_hasher)},
        _m_key_equal{std::move(_other._m_key_equal)},
        _m_allocator{std::move(_other._m_allocator)}
    {}

    // Move constuctor with allocator parameter
    HashMap(HashMap&& _other, const _allocator_t& _alloc):
        _m_buckets{std::move(_other._m_buckets)},
        _m_count{_other._m_count},
        _m_max_load_factor{_other._m_max_load_factor},
        _m_hasher{std::move(_other._m_hasher)},
        _m_key_equal{std::move(_other._m_key_equal)},
        _m_allocator{std::move(_alloc)}
    {}

    // Constructor based on the initialization list
    HashMap
    (
        std::initializer_list<_value_t> _il,
        size_t _count_buckets = MIN_COUNT_BUCKETS,
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
    ~HashMap()
    {}

    ///////////////////////////////////////////////////////////////////////////


    // Assigment operator
    ///////////////////////////////////////////////////////////////////////////

    // Assignment by copying
    HashMap& operator=(const HashMap& _other)
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
    HashMap& operator=(HashMap&& _other) noexcept
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
    HashMap& operator=(std::initializer_list<_value_t> _il)
    {
        clear();
        insert(_il);

        return *this;
    }

    ///////////////////////////////////////////////////////////////////////////


    // Iterators
    ///////////////////////////////////////////////////////////////////////////

    // Returns the iterator set to the beginning of the container
    iterator begin() noexcept
    { return iterator(*this, _m_buckets.begin()); }
    // Returns the const iterator set to the beginning of the container
    const_iterator begin() const noexcept
    { return const_iterator(*this, _m_buckets.cbegin()); }
    // Returns the const iterator set to the beginning of the container
    const_iterator cbegin() const noexcept
    { return const_iterator(*this, _m_buckets.cbegin()); }
    // Returns the iterator set to the end of the container
    iterator end() noexcept
    { return iterator(*this); }
    // Returns the const iterator set to the end of the container
    const_iterator end() const noexcept
    { return const_iterator(*this); }
    // Returns the const iterator set to the end of the container
    const_iterator cend() const noexcept
    { return const_iterator(*this); }

    ///////////////////////////////////////////////////////////////////////////


    // Capacity and size
    ///////////////////////////////////////////////////////////////////////////

    // Count of items in container
    size_t size() const noexcept { return _m_count; }
    // Checking the container for emptiness
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
            return (*iter).second;
        
        // Otherwise, add it to containter

        // If an overflow of the container occurs after the addition,
        // then it must first be expanded
        _bucket_t* buck_ptr = nullptr;
        if (_load_factor(_m_count + 1) > _m_max_load_factor)
        {
            reverse(_m_count * 2);
            buck_ptr = &_m_buckets[bucket(_key)];
        }
        else
            buck_ptr = &iter._m_table_iter;

        buck_ptr->push_front(std::make_pair(_key, _mapped_t{}));
        _m_count++;
        
        return buck_ptr->front().second;
    }

    _mapped_t& operator[](_key_t&& _key)
    {
        iterator iter = find(_key);

        // If an element with such a key was founded, then return it
        if (iter != end())
            return (*iter).second;
        
        // Otherwise, add it to containter

        // If an overflow of the container occurs after the addition,
        // then it must first be expanded
        _bucket_t* buck_ptr = nullptr;
        if (_load_factor(_m_count + 1) > _m_max_load_factor)
        {
            reverse(_m_count * 2);
            buck_ptr = &_m_buckets[bucket(_key)];
        }
        else
            buck_ptr = &iter._m_table_iter;

        buck_ptr->push_front(std::make_pair(_key, _mapped_t{}));
        _m_count++;
        
        return buck_ptr->front().second;
    }

    // Access to the element by key, if the element is not found,
    // an out_of_range exception is thrown
    
    _mapped_t& at(const _key_t& _key)
    {
        iterator iter = find(_key);

        if (iter != end())
            return (*iter).second;
        else
            throw std::out_of_range("the element with this key was not found");
    }

    const _mapped_t& at(const _key_t& _key) const
    {
        const_iterator iter = find(_key);

        if (iter != end())
            return (*iter).second;
        else
            throw std::out_of_range("the element with this key was not found");
    }

    // Accessing an element by key and returning an iterator

    iterator find(const _key_t& _key) noexcept
    {
        size_t i = bucket(_key);
        _table_iterator table_iter = _m_buckets.begin() + i;
        bucket_iterator buck_iter = _m_buckets[i].begin();

        for (; buck_iter != _m_buckets[i].end(); buck_iter++)
            if (_m_key_equal(_key, buck_iter->first))
                return iterator(*this, table_iter, buck_iter);
        
        return iterator(*this);
    }

    const_iterator find(const _key_t& _key) const noexcept
    {
        size_t i = bucket(_key);
        _const_table_iterator table_iter = _m_buckets.cbegin() + i;
        const_bucket_iterator buck_iter = _m_buckets[i].cbegin();

        for (; buck_iter != _m_buckets[i].cend(); buck_iter++)
            if (_m_key_equal(_key, buck_iter->first))
                return const_iterator(*this, table_iter, buck_iter);
        
        return const_iterator(*this);
    }

    // Returns count of items with specified key in container
    // (1 if there is such an element, 0 otherwise)
    size_t count(const _key_t& _key) const noexcept
    { return find(_key) != cend(); }

    ///////////////////////////////////////////////////////////////////////////


    // Modifiers
    ///////////////////////////////////////////////////////////////////////////

    // Insert operations

    // Inserting a single element by copying
    std::pair<iterator, bool> insert(const _value_t& _val)
    {
        iterator iter = find(_val.first);

        // If an element with such a key was founded
        if (iter != end())
            return std::make_pair(iter, false);

        // Otherwise, add it to containter

        // If an overflow of the container occurs after the addition,
        // then it must first be expanded
        if (_load_factor(_m_count + 1) > _m_max_load_factor)
            reverse(_m_count * 2);
        
        _table_iterator table_iter = _m_buckets.begin() + bucket(_val.first);

        (*table_iter).push_front(_val);
        _m_count++;
        
        return std::make_pair
        (
            iterator(*this, table_iter, (*table_iter).begin()),
            true
        );
    }

    // Inserting a single element by moving
    std::pair<iterator, bool> insert(_value_t&& _val)
    {
        iterator iter = find(_val.first);

        // If an element with such a key was founded
        if (iter != end())
            return std::make_pair(iter, false);

        // Otherwise, add it to containter

        // If an overflow of the container occurs after the addition,
        // then it must first be expanded
        if (_load_factor(_m_count + 1) > _m_max_load_factor)
            reverse(_m_count * 2);
        
        _table_iterator table_iter = _m_buckets.begin() + bucket(_val.first);

        (*table_iter).push_front(_val);
        _m_count++;
        
        return std::make_pair
        (
            iterator(*this, table_iter, (*table_iter).begin()),
            true
        );
    }

    // Inserting a range of values
    template <class InputIterator>
    size_t insert(InputIterator _first, InputIterator _last)
    {
        size_t count = std::distance(_first, _last);

        if (_load_factor(_m_count + count) > _m_max_load_factor)
            reverse(count);
        
        size_t result = 0;
        for (InputIterator iter = _first; iter < _last; iter++)
            if (insert(*iter).second)
                result++;
        
        return result;
    }

    // Inserting an initialization list
    size_t insert(std::initializer_list<_value_t> _il)
    {
        size_t count = _il.size();

        if (_load_factor(_m_count + count) > _m_max_load_factor)
            reverse(count);

        size_t result = 0;
        for (auto&& item : _il)
            if (insert(item).second)
                result++;
        
        return result;
    }

    // Erase operations

    // Erase item from container by specified key
    size_t erase(const _key_t& _key)
    {
        iterator iter = find(_key);
        _table_iterator table_iter = iter._m_table_iter;

        size_t old_count = _m_count;

        if (table_iter != _m_buckets.end())
        {
            (*table_iter).remove_if
            (
                [this, &_key](const _value_t& val) -> bool
                {
                    if (_m_key_equal(val.first, _key))
                    {
                        _m_count--;
                        return true;
                    }

                    return false;
                }
            );
        }

        return old_count - _m_count;
    }

    // Clear the container
    void clear()
    {
        _m_buckets.resize(MIN_COUNT_BUCKETS);
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

    // Returns count of buckets in container
    size_t buckets_count() const noexcept { return _m_buckets.size(); }
    // Returns size of specified bucket
    size_t bucket_size(size_t _n) const noexcept
    { return std::distance(_m_buckets[_n].cbegin(), _m_buckets[_n].cend()); }
    // Returns number of bucket by specified key
    size_t bucket(const _key_t& _key) const noexcept
    { return mod_hash(_m_hasher(_key), _m_buckets.size()); }

    ///////////////////////////////////////////////////////////////////////////


    // Hash policy
    ///////////////////////////////////////////////////////////////////////////
    
    // Returns the average number of elements per bucket
    float load_factor() const noexcept
    { return _load_factor(_m_count); }

    // Returns current maximum load factor
    float max_load_factor() const noexcept
    { return _m_max_load_factor; }

    // Set the maximum load factor to specified value
    void max_load_factor(float _ml) noexcept
    { _m_max_load_factor = _ml; }

    // Sets the number of buckets to count and rehashes the container
    void rehash(size_t _count_buckets)
    {
        if (_count_buckets < MIN_COUNT_BUCKETS)
            _count_buckets = MIN_COUNT_BUCKETS;
        
        if (_count_buckets == _m_buckets.size())
            return;
        
        // If the new number of buckets makes load factor more than maximum
        // load factor... 
        if (_m_max_load_factor < ((float)_m_count / _count_buckets))
            // then the new number of buckets is at least:
            _count_buckets = _m_count / _m_max_load_factor;
        
        std::vector<_bucket_t> new_buckets(_count_buckets);

        for (_value_t& value : *this)
        {
            size_t i = mod_hash(_m_hasher(value.first), _count_buckets);
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
    void print(std::ostream& _stream) const noexcept
    {
        _stream << "[i] -> ... -> key{value} -> ..." << std::endl;

        size_t i = 0;
        for (const auto& bucket : _m_buckets)
        {
            _stream << "[" << i << "]: ";

            size_t j = 0;
            for (const auto& item : bucket)
            {
                _stream << "-> " << item.first << "{" << item.second << "} ";
                j++;
            }
            _stream << std::endl;
            
            i++;
        }
    }
    ///////////////////////////////////////////////////////////////////////////

    // Iterator
    class iterator:
        public std::iterator<std::forward_iterator_tag, _value_t>
    {
    private:
        friend class HashMap;

    private:
        HashMap* _m_ht_ptr;
        _table_iterator _m_table_iter;
        bucket_iterator _m_buck_iter;

        // Default constructor
        iterator(HashMap& _table):
            _m_ht_ptr{&_table},
            _m_table_iter{_table._m_buckets.end()},
            _m_buck_iter{}
        {}

        // Constructor with table iterator parameter
        iterator(HashMap& _table, const _table_iterator& _iter):
            _m_ht_ptr{&_table},
            _m_table_iter{_iter}
        {
            if (_m_table_iter == _m_ht_ptr->_m_buckets.end())
                return;
            
            _m_buck_iter = (*_m_table_iter).begin();

            while (_m_buck_iter == (*_m_table_iter).end())
            {
                _m_table_iter++;
                
                if (_m_table_iter != _m_ht_ptr->_m_buckets.end())
                    _m_buck_iter = (*_m_table_iter).begin();
                else
                    break;
            }
        }

        // Constructor with table iterator and bucket iterator parameters
        iterator
        (
            HashMap& _table,
            const _table_iterator& _table_iter,
            const bucket_iterator& _buck_iter
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
        iterator& operator=(const iterator& _other) noexcept = default;
        // Assigment by moving
        iterator& operator=(iterator&& _other) noexcept = default;

        // Equality operator
        bool operator==(const iterator& _other) const noexcept
        {
            // If iterators are installed on the same container and bucket
            if
            (
                _m_ht_ptr == _other._m_ht_ptr && 
                _m_table_iter == _other._m_table_iter
            )
            {
                // If both iterators are not set
                if (_m_table_iter == _m_ht_ptr->_m_buckets.end())
                    return true;
                else
                    return _m_buck_iter == _other._m_buck_iter;
            }
            else
                return false;                   
        }

        // Inequality operator
        bool operator!=(const iterator& _other) const noexcept
        { return !(*this == _other); }

        // Dereference Operator
        _value_t& operator*() const noexcept
        { return *_m_buck_iter; }

        // Prefix increment operator
        iterator& operator++() noexcept
        {
            if (_m_table_iter == _m_ht_ptr->_m_buckets.end())
                return *this;

            _m_buck_iter++;

            while (_m_buck_iter == (*_m_table_iter).end())
            {
                _m_table_iter++;
                
                if (_m_table_iter != _m_ht_ptr->_m_buckets.end())
                    _m_buck_iter = (*_m_table_iter).begin();
                else
                    break;
            }

            return *this;
        }

        // Postfix increment operator
        iterator operator++(int) noexcept
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
        friend class HashMap;

    private:
        const HashMap* _m_ht_ptr;
        _const_table_iterator _m_table_iter;
        const_bucket_iterator _m_buck_iter;

        // Default constructor
        const_iterator(const HashMap& _table):
            _m_ht_ptr{&_table},
            _m_table_iter{_table._m_buckets.cend()},
            _m_buck_iter{}
        {}

        // Constructor with table iterator parameter
        const_iterator
        (
            const HashMap& _table,
            const _const_table_iterator& _iter
        ):
            _m_ht_ptr{&_table},
            _m_table_iter{_iter}
        {
            if (_m_table_iter == _m_ht_ptr->_m_buckets.cend())
                return;
            
            _m_buck_iter = (*_m_table_iter).cbegin();

            while (_m_buck_iter == (*_m_table_iter).cend())
            {
                _m_table_iter++;
                
                if (_m_table_iter != (*_m_ht_ptr)._m_buckets.cend())
                    _m_buck_iter = (*_m_table_iter).cbegin();
                else
                    break;
            }
        }

        // Constructor with table iterator and bucket iterator parameters
        const_iterator
        (
            const HashMap& _table,
            const _const_table_iterator& _table_iter,
            const const_bucket_iterator& _buck_iter
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
        const_iterator& operator=(const const_iterator& _other) noexcept
            = default;
        // Assigment by moving
        const_iterator& operator=(const_iterator&& _other) noexcept
            = default;

        // Equality operator
        bool operator==(const const_iterator& _other) const noexcept
        {
            // If iterators are installed on the same container and bucket
            if
            (
                _m_ht_ptr == _other._m_ht_ptr && 
                _m_table_iter == _other._m_table_iter
            )
            {
                // If both iterators are not set
                if (_m_table_iter == _m_ht_ptr->_m_buckets.cend())
                    return true;
                else
                    return _m_buck_iter == _other._m_buck_iter;
            }
            else
                return false;
        }

        // Inequality operator
        bool operator!=(const const_iterator& _other) const noexcept
        { return !(*this == _other); }

        // Dereference Operator
        const _value_t& operator*() const noexcept
        { return *_m_buck_iter; }

        // Prefix increment operator
        const_iterator& operator++() noexcept
        {
            if (_m_table_iter == (*_m_ht_ptr)._m_buckets.cend())
                return *this;

            _m_buck_iter++;

            while (_m_buck_iter == (*_m_table_iter).cend())
            {
                _m_table_iter++;
                
                if (_m_table_iter != (*_m_ht_ptr)._m_buckets.cend())
                    _m_buck_iter = (*_m_table_iter).cbegin();
                else
                    break;
            }

            return *this;
        }

        // Postfix increment operator
        const_iterator operator++(int) noexcept
        {
            const_iterator temp = *this;
            ++(*this);

            return temp;
        }

    };
    ///////////////////////////////////////////////////////////////////////////

}; // HashMap


#endif  // _HASHMAP_
