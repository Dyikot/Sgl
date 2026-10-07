#pragma once

#include <vector>
#include <ranges>
#include <concepts>
#include <algorithm>

namespace Sgl
{
    //! @brief A generic dynamic array wrapper around std::vector
    //! @tparam T The type of elements stored in the vector
    template <typename T>
    class Vector
    {
    public:
        using iterator = typename std::vector<T>::iterator;
        using const_iterator = typename std::vector<T>::const_iterator;
        using size_type = typename std::vector<T>::size_type;
    public:
        //! @brief Default constructor. Creates an empty vector.
        Vector() = default;

        //! @brief Copy constructor
        //! @param other The vector to copy from
        Vector(const Vector& other) = default;

        //! @brief Move constructor
        //! @param other The vector to move from
        Vector(Vector&& other) noexcept = default;

        //! @brief Constructs a vector from an initializer list
        //! @param init The initializer list containing the elements
        Vector(std::initializer_list<T> init): _data(init) {}

        //! @brief Constructs a vector from an existing std::vector
        //! @param vec The vector
        explicit Vector(std::vector<T> vec): _data(std::move(vec)) {}

        //! @brief Returns an iterator to the beginning
        //! @return Iterator to the first element
        iterator begin() noexcept
        {
            return _data.begin();
        }

        //! @brief Returns a const iterator to the beginning
        //! @return Const iterator to the first element
        const_iterator begin() const noexcept
        {
            return _data.begin();
        }

        //! @brief Returns an iterator to the end
        //! @return Iterator past the last element
        iterator end() noexcept
        {
            return _data.end();
        }

        //! @brief Returns a const iterator to the end
        //! @return Const iterator past the last element
        const_iterator end() const noexcept
        {
            return _data.end();
        }

        //! @brief Adds an element to the end of the vector
        //! @param value The value to add
        void Add(const T& value)
        {
            _data.push_back(value);
        }

        //! @brief Adds an element to the end of the vector
        //! @param value The value to add
        void Add(T&& value)
        {
            _data.push_back(std::move(value));
        }

        //! @brief Appends all elements from range to the end
        //! @tparam Range A type satisfying std::ranges::input_range
        //! @param range The range whose elements will be appended
        template <std::ranges::input_range TRange>
            requires std::convertible_to<std::ranges::range_reference_t<TRange>, T>
        void AddRange(TRange&& range)
        {
            _data.insert(_data.end(), std::ranges::begin(range), std::ranges::end(range));
        }

        //! @brief Inserts an element at the specified index
        //! @param index The index where the element should be inserted
        //! @param value The value to insert
        void Insert(size_type index, const T& value)
        {
            _data.insert(_data.begin() + index, value);
        }

        //! @brief Inserts an element at the specified index
        //! @param index The index where the element should be inserted
        //! @param value The value to insert
        void Insert(size_type index, T&& value)
        {
            _data.insert(_data.begin() + index, std::move(value));
        }

        //! @brief Inserts all elements range at the specified index
        //! @tparam Range A type satisfying std::ranges::input_range
        //! @param range The range whose elements will be appended
        template <std::ranges::input_range TRange>
            requires std::convertible_to<std::ranges::range_reference_t<TRange>, T>
        void InsertRange(size_type index, TRange&& range)
        {
            _data.insert(_data.begin() + index, std::ranges::begin(range), std::ranges::end(range));
        }

        //! @brief Removes the first occurrence of a specific value
        //! @param value The value to remove
        //! @return True if the element was found and removed; otherwise, false
        bool Remove(const T& value)
        {
            auto it = std::ranges::find(_data, value);
            if(it != _data.end())
            {
                _data.erase(it);
                return true;
            }

            return false;
        }

        //! @brief Removes the element at the specified index
        //! @param index The zero-based index of the element to remove
        void RemoveAt(size_type index)
        {
            _data.erase(_data.begin() + index);
        }

        //! @brief Reserves storage for at least the specified number of elements
        //! @param capacity The minimum number of elements to reserve space for
        void Reserve(size_type capacity)
        {
            _data.reserve(capacity);
        }

        //! @brief Resizes the vector to contain the specified number of elements
        //! @param count The new size of the vector
        void Resize(size_type count)
        {
            _data.resize(count);
        }

        //! @brief Removes all elements from the vector
        void Clear() noexcept
        {
            _data.clear();
        }

        //! @brief Checks whether the vector contains no elements
        //! @return True if the vector is empty; otherwise, false
        bool IsEmpty() const noexcept
        {
            return _data.empty();
        }

        //! @brief Gets the number of elements in the vector
        //! @return The number of elements
        size_type Count() const noexcept
        {
            return _data.size();
        }

        //! @brief Determines whether the vector contains a specific value
        //! @param value The value to locate
        //! @return True if the value is found; otherwise, false
        bool Contains(const T& value) const
        {
            return std::ranges::find(_data, value) != _data.end();
        }

        //! @brief Copy assignment operator
        //! @param other The vector to copy from
        //! @return A reference to this vector
        Vector& operator=(const Vector& other) = default;

        //! @brief Move assignment operator
        //! @param other The vector to move from
        //! @return A reference to this vector
        Vector& operator=(Vector&& other) noexcept = default;

        //! @brief Equality operator
        //! @param other The vector to compare against
        //! @return True if both vectors contain the same elements in the same order
        bool operator==(const Vector& other) const
        {
            return _data == other._data;
        }

        //! @brief Subscript operator for element access
        //! @param index The zero-based index of the element
        //! @return A reference to the element at the given index
        T& operator[](size_type index)
        {
            return _data[index];
        }

        //! @brief Subscript operator for read-only element access
        //! @param index The zero-based index of the element
        //! @return A const reference to the element at the given index
        const T& operator[](size_type index) const
        {
            return _data[index];
        }

    protected:
        std::vector<T> _data;
    };
}