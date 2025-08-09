#pragma once

#include <atomic>
#include <memory>
#include <type_traits>
#include <utility>
#include <concepts>
#include <stdexcept>

namespace Sgl
{
    template<typename T>
    using Unique = std::unique_ptr<T>;    

    template<typename T>
    using Shared = std::shared_ptr<T>;    

    template<typename T>
    class Ref
    {
    private:
        Shared<T> _ptr;
    public:
        Ref(Shared<T> ptr):
            _ptr(std::move(ptr))
        {
            if(_ptr == nullptr)
            {
                throw std::invalid_argument("Ref cannot be constructed from nullptr");
            }
        }

        template<typename TBase>
        Ref(const Ref<TBase>& other):
            _ptr(std::static_pointer_cast<T>(other.AsShared()))
        {
            if(_ptr == nullptr)
            {
                throw std::invalid_argument("Ref cannot be constructed from nullptr");
            }
        }

        template<typename TBase>
        Ref(Ref<TBase>&& other) noexcept:
            _ptr(std::static_pointer_cast<T>(std::move(other.AsShared())))
        {
            if(_ptr == nullptr)
            {
                throw std::invalid_argument("Ref cannot be constructed from nullptr");
            }
        }

        Ref(const Ref& other):
            _ptr(other._ptr)
        {}

        Ref(Ref&& other) noexcept: 
            _ptr(std::move(other._ptr))
        {}

        ~Ref() = default;

        T& Value()
        {
            return *_ptr;
        }

        const T& Value() const
        {
            return *_ptr;
        }

        T* Pointer()
        {
            return _ptr.get();
        }

        const T* Pointer() const
        {
            return _ptr.get();
        }

        const Shared<T>& AsShared() const
        {
            return _ptr;
        }

        Ref& operator=(T* ptr)
        {
            if(ptr == nullptr)
            {
                throw std::invalid_argument("Ref cannot be constructed from nullptr");
            }

            _ptr.reset(ptr);
            return *this;
        }

        Ref& operator=(const Ref& other)
        {
            if(this != &other)
            {
                _ptr = other._ptr;
            }

            return *this;
        }

        Ref& operator=(Ref&& other) noexcept
        {
            if(this != &other)
            {
                _ptr = std::move(other._ptr);
            }

            return *this;
        }

        template<typename TBase>
        Ref& operator=(const Ref<TBase>& other)
        {
            _ptr = std::static_pointer_cast<T>(other.AsShared());
            return *this;
        }

        template<typename TBase>
        Ref& operator=(Ref<TBase>&& other) noexcept
        {
            _ptr = std::static_pointer_cast<T>(std::move(other.AsShared()));
            return *this;
        }

        operator T& ()
        {
            return *_ptr;
        }

        operator const T& () const
        {
            return *_ptr;
        }

        T* operator->()
        {
            return _ptr.get();
        }

        const T* operator->() const
        {
            return _ptr.get();
        }        
    };

    template<typename TValue, typename... TArgs>
    Unique<TValue> NewUnique(TArgs&&... args)
    {
        return std::make_unique<TValue>(std::forward<TArgs>(args)...);
    }

    template<typename TValue, typename... TArgs>
    Shared<TValue> NewShared(TArgs&&... args)
    {
        return std::make_shared<TValue>(std::forward<TArgs>(args)...);
    }

    template<typename TValue, typename... TArgs>
    Ref<TValue> NewRef(TArgs&&... args)
    {
        return Ref<TValue>(NewShared<TValue>(std::forward<TArgs>(args)...));
    }

    template<typename T, typename TConfigurer>
    Unique<T> CreateUnique(TConfigurer&& configurer)
    {
        auto element = NewUnique<T>();
        configurer(*element);
        return element;
    }

    template<typename T, typename TConfigurer>
    Shared<T> CreateShared(TConfigurer&& configurer)
    {
        auto element = NewShared<T>();
        configurer(*element);
        return element;
    }

    template<typename T, typename TConfigurer>
    Ref<T> CreateRef(TConfigurer&& configurer)
    {
        auto element = NewRef<T>();
        configurer(*element);
        return element;
    }
}