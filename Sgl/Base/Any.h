#pragma once

#include <cassert>
#include <concepts>
#include <typeinfo>
#include <utility>

namespace Sgl
{    
    //! @brief A type-safe container that can hold a single value of any copyable type.
    //! Unlike std::any, supports equality comparison of contained values.
    //! Values are always heap-allocated (no small-buffer optimization).
    class Any
    {
    private:
        class IStorage
        {
        public:
            virtual ~IStorage() = default;

            virtual IStorage* Copy() const = 0;
            virtual const std::type_info& Type() const noexcept = 0;

            template<typename T>
            T& Get() noexcept
            {
                return static_cast<Storage<T>*>(this)->Value;
            }

            template<typename T>
            const T& Get() const noexcept
            {
                return static_cast<const Storage<T>*>(this)->Value;
            }

            virtual bool operator==(const IStorage& other) const = 0;
        };

        template<typename T>
        class Storage: public IStorage
        {
        public:
            template<typename... TArgs>
            Storage(TArgs&&... args):
                Value(std::forward<TArgs>(args)...)
            {}

            T Value;

            IStorage* Copy() const override
            {
                return new Storage<T>(Value);
            }

            const std::type_info& Type() const noexcept override
            {
                return typeid(T);
            }

            bool operator==(const IStorage& other) const override
            {
                if(Type() != other.Type())
                {
                    return false;
                }

                if constexpr(std::equality_comparable<T>)
                {
                    const auto& otherStorage = static_cast<const Storage<T>&>(other);
                    return Value == otherStorage.Value;
                }

                return false;
            }
        };

    public:
        Any() = default;

        Any(const Any& other):
            _data(other._data ? other._data->Copy() : nullptr)
        {}

        Any(Any&& other) noexcept:
            _data(std::exchange(other._data, nullptr))
        {}

        template<typename T>
        Any(T&& value):
            _data(new Storage<std::decay_t<T>>(std::forward<T>(value)))
        {}

        ~Any()
        {
            delete _data;
        }

        //! @brief Constructs an object of type T in-place inside the storage
        template<typename T, typename... TArgs>
        std::decay_t<T>& Emplace(TArgs&&... args)
        {
            IStorage* newData = new Storage<std::decay_t<T>>(std::forward<TArgs>(args)...);
            std::swap(_data, newData);
            delete newData;
            return _data->Get<std::decay_t<T>>();
        }

        //! @brief Retrieves a reference to the stored value as T
        template<typename T>
        T& As()
        {
            assert(Is<T>());
            return _data->Get<T>();
        }
        
        //! @brief Retrieves a const reference to the stored value as T
        template<typename T>
        const T& As() const
        {
            assert(Is<T>());
            return _data->Get<T>();
        }

        //! @brief Checks whether the contained value is of type T
        template<typename T>
        bool Is() const noexcept
        {
            return _data && _data->Type() == typeid(T);
        }

        //! @brief Checks whether the contained value matches the given type_info
        bool Is(const std::type_info& typeInfo) const noexcept
        {
            return _data && _data->Type() == typeInfo;
        }

        //! @brief Returns true if this Any holds a value
        bool HasValue() const noexcept
        {
            return _data != nullptr;
        }
        
        //! @brief Resets the object to its initial state.
        void Reset()
        {
            delete _data;
            _data = nullptr;
        }

        //! @brief Swap the value with another object
        void Swap(Any& other)
        {
            std::swap(_data, other._data);
        }

        template<typename T>
        Any& operator=(T&& value)
        {
            Emplace<std::decay_t<T>>(std::forward<T>(value));
            return *this;
        }

        Any& operator=(const Any& other)
        {
            if(this != &other)
            {
                Any tmp(other);
                Swap(tmp);
            }

            return *this;
        }

        Any& operator=(Any&& other) noexcept
        {
            if(this != &other)
            {
                delete _data;
                _data = std::exchange(other._data, nullptr);
            }

            return *this;
        }
        
        //! @brief True if both are empty, or same type and equal values.
        //! Different types and non-comparable types compare unequal.
        friend bool operator==(const Any& left, const Any& right)
        {
            if(!left._data || !right._data)
            {
                return left._data == right._data;
            }

            return *left._data == *right._data;
        }

    private:
        IStorage* _data = nullptr;
    };
}