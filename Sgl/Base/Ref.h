#pragma once

#include <atomic>
#include <memory>
#include <utility>
#include <typeinfo>
#include <type_traits>

namespace Sgl
{
    /// <summary>
    /// Base class for all objects managed by intrusive Ref<T>
    /// </summary>
    class RefCounted
    {
    public:
        RefCounted() = default;
        RefCounted(const RefCounted&) = delete;
        RefCounted(RefCounted&&) = delete;

        RefCounted& operator=(const RefCounted&) = delete;
        RefCounted& operator=(RefCounted&&) = delete;

    protected:
        virtual ~RefCounted() = default;

        /// <summary>
        /// Increments the reference count.
        /// </summary>
        void AddRef() noexcept
        {
            _references.fetch_add(1, std::memory_order_relaxed);
        }

        /// <summary>
        /// Decrements the reference count. If it hits zero, the object deletes itself.
        /// </summary>
        void Release() noexcept
        {
            if(_references.fetch_sub(1, std::memory_order_acq_rel) == 1)
            {
                delete this;
            }
        }
    private:
        std::atomic<uint32_t> _references = 1;

        template<typename T>
        friend class Ref;
    };

    /// <summary>
    /// A thread-safe, intrusive reference-counted handle for managing shared ownership of an object of type T.
    /// </summary>
    template<typename T>
    class Ref final
    {
    public:
        /// <summary>
        /// Constructs an empty reference.
        /// </summary>
        Ref() noexcept = default;

        /// <summary>
        /// Constructs an empty reference explicitly from nullptr.
        /// </summary>
        Ref(std::nullptr_t) noexcept {}

        /// <summary>
        /// Constructs a reference from another Ref data
        /// </summary>
        explicit Ref(T* data) noexcept:
            _data(data)
        {
            AddRef();
        }

        /// <summary>
        /// Copy-constructs a reference from another Ref instance, incrementing the shared reference count.
        /// </summary>
        Ref(const Ref& other) noexcept: 
            _data(other._data)
        {
            AddRef();
        }

        /// <summary>
        /// Copy-constructs a reference from a Ref of a derived type (safe upcasting).
        /// </summary>
        template<std::derived_from<T> TDerived>
        Ref(const Ref<TDerived>& other) noexcept: 
            _data(other._data)
        {
            AddRef();
        }

        /// <summary>
        /// Move-constructs a reference, transferring ownership without modifying the reference count.
        /// </summary>
        Ref(Ref&& other) noexcept: 
            _data(other._data)
        {
            other._data = nullptr;
        }

        /// <summary>
        /// Move-constructs a reference from a Ref of a derived type (safe upcasting with move).
        /// </summary>
        template<std::derived_from<T> TDerived>
        Ref(Ref<TDerived>&& other) noexcept:
            _data(other._data)
        {
            other._data = nullptr;
        }

        /// <summary>
        /// Destroys this reference. If it was the last owner, the object is deleted.
        /// </summary>
        ~Ref()
        {
            static_assert(std::derived_from<T, RefCounted>,
                          "Ref<T> requires T to be derived from RefCounted");
            Release();
        }
        
        /// <summary>
        /// Performs a safe downcast to a derived type TDerived, returning a new Ref<TDerived>.
        /// </summary>
        template<std::derived_from<T> TDerived>
        Ref<TDerived> As() const
        {
            if(auto casted = dynamic_cast<TDerived*>(_data))
            {
                return Ref<TDerived>(casted);
            }

            return nullptr;
        }

        /// <summary>
        /// Checks whether the managed object is of (or derives from) type TDerived.
        /// </summary>
        template<std::derived_from<T> TDerived>
        bool Is() const
        {
            return dynamic_cast<TDerived*>(_data) != nullptr;
        }

        /// <summary>
        /// Checks whether the managed object is exactly of type U.
        /// </summary>        
        template<typename U>
        bool OfType() const
        {
            return _data && typeid(*_data) == typeid(U);
        }

        /// <summary>
        /// Returns a raw pointer to the managed object, or nullptr if empty.
        /// </summary>
        T* Get() const noexcept
        {
            return _data;
        }

        /// <summary>
        /// Returns a raw pointer to the managed object cast to type TOther*.
        /// No runtime checks are performed.
        /// </summary>
        template<typename TOther>
        TOther* GetAs() const
        {
            return static_cast<TOther*>(_data);
        }

        /// <summary>
        /// Returns a reference to the managed object.
        /// </summary>
        T& GetValue() const
        {
            return *_data;
        }

        /// <summary>
        /// Returns a reference to the managed object cast to type TValue.
        /// </summary>
        template<typename TValue>
        TValue& GetValueAs() const
        {
            return *static_cast<TValue*>(_data);
        }

        void Reset() noexcept
        {
            Release();
            _data = nullptr;
        }

        /// <summary>
        /// Swaps the contents of this reference with another.
        /// </summary>
        void swap(Ref& other) noexcept
        {
            std::swap(_data, other._data);
        }

        T* operator->() const noexcept
        {
            return _data;
        }

        T& operator*() const noexcept
        {
            return *_data;
        }

        Ref& operator=(std::nullptr_t)
        {
            Release();
            _data = nullptr;
            return *this;
        }

        Ref& operator=(T* data) noexcept
        {
            Release();
            _data = data;
            AddRef();
            return *this;
        }

        Ref& operator=(const Ref& other) noexcept
        {
            if(this != &other)
            {
                Release();
                _data = other._data;
                AddRef();
            }

            return *this;
        }

        template<std::derived_from<T> TDerived>
        Ref& operator=(const Ref<TDerived>& other) noexcept
        {
            Release();
            _data = other._data;
            AddRef();
            return *this;
        }

        Ref& operator=(Ref&& other) noexcept
        {
            if(this != &other)
            {
                Release();
                _data = other._data;
                other._data = nullptr;
            }

            return *this;
        }

        template<std::derived_from<T> TDerived>
        Ref& operator=(Ref<TDerived>&& other) noexcept
        {
            Release();
            _data = other._data;
            other._data = nullptr;
            return *this;
        }

        explicit operator bool() const noexcept
        {
            return _data != nullptr;
        }

        friend bool operator==(const Ref& left, const Ref& right) noexcept
        {
            return left._data == right._data;
        }
    private:
        Ref(T* data, std::in_place_t) noexcept: 
            _data(data) 
        {}

        void AddRef() noexcept
        {
            if(_data)
            {
                _data->AddRef();
            }
        }

        void Release() noexcept
        {
            if(_data)
            {
                _data->Release();
                _data = nullptr;
            }
        }

        template<typename U>
        friend class Ref;

        template<typename U, typename... TArgs>
        friend Ref<U> New(TArgs&&... args);

    private:
        T* _data = nullptr;
    };

    /// <summary>
    /// Creates a new Ref<T> by constructing an object of type T with the given arguments.
    /// </summary>
    template<typename T, typename... TArgs>
    Ref<T> New(TArgs&&... args)
    {
        return Ref<T>(new T(std::forward<TArgs>(args)...), std::in_place);
    }
}