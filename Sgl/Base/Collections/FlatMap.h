#pragma once

#include <algorithm>
#include <ranges>
#include <vector>
#include <utility>

namespace Sgl
{
    //! A simple key-value container backed by two parallel vectors
    //! @tparam TKey Key type, must be equality comparable
    //! @tparam TValue Value type
    template <std::equality_comparable TKey, typename TValue>
    class FlatMap
    {
    public:
        class const_iterator
        {
        public:
            using iterator_category = std::random_access_iterator_tag;
            using iterator_concept = std::random_access_iterator_tag;

            using value_type = std::pair<const TKey, TValue>;
            using difference_type = std::ptrdiff_t;
            using reference = std::pair<const TKey&, const TValue&>;

            struct pointer
            {
                reference p;

                const reference* operator->() const noexcept
                {
                    return &p;
                }
            };
        public:
            const_iterator() = default;

            const_iterator(const FlatMap* map, std::size_t index)
                : _map(map), _index(static_cast<difference_type>(index))
            {}

            reference operator*() const
            {
                return reference { _map->_keys[_index], _map->_values[_index] };
            }

            pointer operator->() const
            {
                return pointer { **this };
            }

            const_iterator& operator++()
            {
                ++_index;
                return *this;
            }

            const_iterator operator++(int)
            {
                auto tmp = *this;
                ++_index;
                return tmp;
            }

            const_iterator& operator--()
            {
                --_index;
                return *this;
            }

            const_iterator operator--(int)
            {
                auto tmp = *this;
                --_index;
                return tmp;
            }

            const_iterator& operator+=(difference_type n)
            {
                _index += n;
                return *this;
            }

            const_iterator& operator-=(difference_type n)
            {
                _index -= n;
                return *this;
            }

            friend const_iterator operator+(const_iterator it, difference_type n)
            {
                it += n;
                return it;
            }

            friend const_iterator operator+(difference_type n, const_iterator it)
            {
                it += n;
                return it;
            }

            friend const_iterator operator-(const_iterator it, difference_type n)
            {
                it -= n;
                return it;
            }

            friend difference_type operator-(const_iterator lhs, const_iterator rhs)
            {
                return lhs._index - rhs._index;
            }

            reference operator[](difference_type n) const
            {
                return *(*this + n);
            }

            bool operator==(const const_iterator&) const = default;
            auto operator<=>(const const_iterator&) const = default;

        private:           
            const FlatMap* _map;
            std::size_t _index;
        };

    public:
        FlatMap() = default;
        FlatMap(const FlatMap&) = default;
        FlatMap(FlatMap&&) = default;

        FlatMap(std::initializer_list<std::pair<TKey, TValue>> list)
        {
            _keys.reserve(list.size());
            _values.reserve(list.size());

            for(auto& [key, value] : list)
            {
                _keys.push_back(key);
                _values.push_back(value);
            }
        }

        //! Returns a const iterator to the first element
        const_iterator begin() const { return const_iterator(this, 0); }

        //! Returns a const iterator past the last element
        const_iterator end() const { return const_iterator(this, _keys.size()); }

        //! Adds a key-value pair
        //! If the key already exists, its value is replaced
        //! @param key The key to add or update
        //! @param value The value to associate with the key
        void Add(const TKey& key, const TValue& value)
        {
            if(auto it = std::ranges::find(_keys, key); it != _keys.end())
            {
                auto index = std::distance(_keys.begin(), it);
                _values[index] = value;
                return;
            }

            _keys.push_back(key);
            _values.push_back(value);
        }

        //! Adds a key-value pair
        //! If the key already exists, its value is replaced
        //! @param key The key to add or update
        //! @param value The value to associate with the key
        void Add(const TKey& key, TValue&& value)
        {
            if(auto it = std::ranges::find(_keys, key); it != _keys.end())
            {
                auto index = std::distance(_keys.begin(), it);
                _values[index] = std::move(value);
                return;
            }

            _keys.push_back(key);
            _values.push_back(std::move(value));
        }

        //! Finds the value associated with the given key
        //! @param key The key to look for
        //! @return Pointer to the value, or nullptr if the key is not present
        TValue* Find(const TKey& key)
        {
            auto it = std::ranges::find(_keys, key);
            return it != _keys.end() ? &_values[it - _keys.begin()] : nullptr;
        }

        //! Finds the value associated with the given key
        //! @param key The key to look for
        //! @return Pointer to the value, or nullptr if the key is not present
        const TValue* Find(const TKey& key) const
        {
            return const_cast<FlatMap&>(*this).Find(key);
        }

        //! Removes the element with the given key
        //! @param key The key to remove
        //! @return True if the key was found and removed, false otherwise
        bool Remove(const TKey& key)
        {
            auto it = std::ranges::find(_keys, key);
            if(it == _keys.end())
            {
                return false;
            }

            auto index = std::distance(_keys.begin(), it);
            _keys.erase(_keys.begin() + index);
            _values.erase(_values.begin() + index);

            return true;
        }

        //! Checks whether the given key is present
        //! @param key The key to check
        //! @return True if the key exists, false otherwise
        bool Contains(const TKey& key) const
        {
            return std::ranges::find(_keys, key) != _keys.end();
        }

        //! Returns the number of key-value pairs
        //! @return The number of stored elements
        std::size_t Size() const noexcept
        {
            return _keys.size();
        }

        //! Checks whether the container is empty
        //! @return True if empty, false otherwise
        bool IsEmpty() const noexcept
        {
            return _keys.empty();
        }

        //! Removes all key-value pairs
        void Clear() noexcept
        {
            _keys.clear();
            _values.clear();
        }

        //! Returns direct access to the underlying keys vector
        //! @return Const reference to the keys, in insertion order
        const std::vector<TKey>& Keys() const noexcept 
        {
            return _keys; 
        }

        //! Returns direct access to the underlying values vector
        //! @return Const reference to the values, in insertion order
        const std::vector<TValue>& Values() const noexcept 
        { 
            return _values; 
        }

        TValue& operator[](const TKey& key)
        {
            auto it = std::ranges::find(_keys, key);

            if(it != _keys.end())
            {
                auto index = std::distance(_keys.begin(), it);
                return _values[index];
            }

            _keys.push_back(key);
            return _values.emplace_back();
        }

        FlatMap& operator=(const FlatMap&) = default;
        FlatMap& operator=(FlatMap&&) noexcept = default;

    private:
        std::vector<TKey> _keys;
        std::vector<TValue> _values;
    };
}