#pragma once

#include <algorithm>
#include <ranges>
#include <vector>
#include <utility>

namespace Sgl
{
    //! A simple key-value container backed by a single contiguous vector of pairs
    //! @tparam TKey Key type, must be equality comparable
    //! @tparam TValue Value type
    template <std::equality_comparable TKey, typename TValue>
    class PackedMap
    {
    public:
        using iterator = typename std::vector<std::pair<TKey, TValue>>::iterator;
        using const_iterator = typename std::vector<std::pair<TKey, TValue>>::const_iterator;

    public:
        PackedMap() = default;
        PackedMap(const PackedMap&) = default;
        PackedMap(PackedMap&&) = default;

        PackedMap(std::initializer_list<std::pair<TKey, TValue>> list):
            _data(list)
        {
        }

        //! Returns a const iterator to the first element.
        const_iterator begin() const { return _data.begin(); }

        //! Returns a const iterator past the last element.
        const_iterator end() const { return _data.end(); }

        //! Returns an iterator to the first element.
        iterator begin() { return _data.begin(); }

        //! Returns an iterator past the last element.
        iterator end() { return _data.end(); }

        //! Adds a key-value pair
        //! If the key already exists, its value is replaced
        //! @param key The key to add or update
        //! @param value The value to associate with the key
        void Add(const TKey& key, const TValue& value)
        {
            auto it = std::ranges::find(_data, key, &std::pair<TKey, TValue>::first);
            if(it != _data.end())
            {
                it->second = value;
                return;
            }

            _data.emplace_back(key, value);
        }

        //! Adds a key-value pair
        //! If the key already exists, its value is replaced
        //! @param key The key to add or update
        //! @param value The value to associate with the key
        void Add(const TKey& key, TValue&& value)
        {
            auto it = std::ranges::find(_data, key, &std::pair<TKey, TValue>::first);
            if(it != _data.end())
            {
                it->second = std::move(value);
                return;
            }

            _data.emplace_back(key, std::move(value));
        }

        //! Finds the value associated with the given key
        //! @param key The key to look for
        //! @return Pointer to the value, or nullptr if the key is not present
        TValue* Find(const TKey& key)
        {
            auto it = std::ranges::find(_data, key, &std::pair<TKey, TValue>::first);
            return it != _data.end() ? &it->second : nullptr;
        }

        //! Finds the value associated with the given key
        //! @param key The key to look for
        //! @return Pointer to the value, or nullptr if the key is not present
        const TValue* Find(const TKey& key) const
        {
            return const_cast<PackedMap&>(*this).Find(key);
        }

        //! Removes the element with the given key
        //! @param key The key to remove
        //! @return True if the key was found and removed, false otherwise
        bool Remove(const TKey& key)
        {
            auto it = std::ranges::find(_data, key, &std::pair<TKey, TValue>::first);
            if(it == _data.end())
            {
                return false;
            }

            _data.erase(it);
            return true;
        }

        //! Checks whether the given key is present
        //! @param key The key to check
        //! @return True if the key exists, false otherwise
        bool Contains(const TKey& key) const
        {
            return std::ranges::find(_data, key, &std::pair<TKey, TValue>::first) != _data.end();
        }

        //! Returns the number of key-value pairs
        //! @return The number of stored elements
        std::size_t Size() const noexcept
        {
            return _data.size();
        }

        //! Checks whether the container is empty
        //! @return True if empty, false otherwise
        bool IsEmpty() const noexcept
        {
            return _data.empty();
        }

        //! Removes all key-value pairs
        void Clear() noexcept
        {
            _data.clear();
        }

        PackedMap& operator=(const PackedMap&) = default;
        PackedMap& operator=(PackedMap&&) noexcept = default;

    private:
        std::vector<std::pair<TKey, TValue>> _data;
    };
}