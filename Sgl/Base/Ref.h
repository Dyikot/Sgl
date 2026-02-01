#pragma once

#include <atomic>
#include <memory>
#include <utility>
#include <stdexcept>

namespace Sgl
{
    struct RefMemoryBlockBase
    {
        virtual ~RefMemoryBlockBase() = default;

        std::atomic<size_t> References = 1;
    };

    template<typename T>
    struct RefMemoryBlock : RefMemoryBlockBase
    {
        template<typename... TArgs>
        RefMemoryBlock(TArgs&&... args):
            Value(std::forward<TArgs>(args)...)
        {}

        T Value;
    };

    /// <summary>
    /// A thread-safe, non-intrusive reference-counted handle for managing shared ownership of an object of type T.
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
        /// Constructs a new object of type T in-place using the provided arguments.
        /// </summary>
        template<typename... TArgs>
        Ref(std::in_place_t, TArgs&&... args):
            _memoryBlock(new RefMemoryBlock<T>(std::forward<TArgs>(args)... )),
            _data(&static_cast<RefMemoryBlock<T>*>(_memoryBlock)->Value)
        {}

        /// <summary>
        /// Copy-constructs a reference from another Ref instance, incrementing the shared reference count.
        /// </summary>
        Ref(const Ref& other) noexcept
        {
            CopyConstructFrom(other);
        }

        /// <summary>
        /// Copy-constructs a reference from a Ref of a derived type (or compatible void*), enabling safe upcasting.
        /// </summary>
        template<typename TDerived> requires
            std::derived_from<TDerived, T> || std::same_as<T, void>
        Ref(const Ref<TDerived>& other) noexcept
        {
            CopyConstructFrom(other);
        }

        /// <summary>
        /// Move-constructs a reference from another Ref instance, transferring ownership without modifying the reference count.
        /// </summary>
        Ref(Ref&& other) noexcept
        {
            MoveConstructFrom(std::move(other));
        }

        /// <summary>
        /// Move-constructs a reference from a Ref of a derived type, enabling safe upcasting with move semantics.
        /// </summary>
        template<typename TDerived> requires
            std::derived_from<TDerived, T> || std::same_as<T, void>
        Ref(Ref<TDerived>&& other) noexcept
        {
            MoveConstructFrom(std::move(other));
        }

        /// <summary>
        /// Constructs a reference to a derived object from a base reference, using an externally provided pointer to the derived type.
        /// Used for downcasting while preserving shared ownership.
        /// </summary>
        template<typename TBase> requires std::is_base_of_v<TBase, T>
        Ref(const Ref<TBase>& base, T* data) noexcept
        {
            CopyConstructFromBase(base, data);
        }

        /// <summary>
        /// Move-constructs a reference to a derived object from a base reference, using an externally provided pointer.
        /// Transfers ownership from the base reference while enabling safe downcasting.
        /// </summary>
        template<typename TBase> requires std::is_base_of_v<TBase, T>
        Ref(Ref<TBase>&& base, T* data) noexcept
        {
            MoveConstructFromBase(std::move(base), data);
        }

        /// <summary>
        /// Destroys this reference. If it was the last owner, the control block are deallocated.
        /// </summary>
        ~Ref()
        {
            Release();            
        }

        /// <summary>
        /// Creates a new Ref<T> by constructing an object of type T with the given arguments.
        /// </summary>
        template<typename T, typename... TArgs>
        friend Ref<T> New(TArgs&&... args);

        /// <summary>
        /// Performs a safe downcast to a derived type TDerived, returning a new Ref<TDerived> that shares ownership.
        /// </summary>
        template<typename TDerived> 
        Ref<TDerived> As() const
        {
            return Ref<TDerived>(*this, GetAs<TDerived>());
        }

        /// <summary>
        /// Checks whether the managed object is of (or derives from) type TDerived.
        /// </summary>
        template<std::derived_from<T> TDerived>
        bool Is() const
        {
            return dynamic_cast<TDerived*>(_data);
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
        /// No runtime checks are performed—use only when the type relationship is known.
        /// </summary>
        template<typename TOther>
        TOther* GetAs() const
        {
            return static_cast<TOther*>(_data);
        }
        
        /// <summary>
        /// Returns a reference to the managed object.
        /// </summary>
        template<typename TValue = T> requires !std::is_void_v<TValue>
        TValue& GetValue() const
        {
            return *_data;
        }

        /// <summary>
        /// Returns a reference to the managed object cast to type TValue.
        /// </summary>
        template<typename TValue> requires !std::is_void_v<TValue>
        TValue& GetValueAs() const
        {
            return static_cast<TValue&>(*_data);
        }

        /// <summary>
        /// Swaps the contents of this reference with another.
        /// </summary>
        void swap(Ref& other)
        {
            std::swap(_data, other._data);
            std::swap(_memoryBlock, other._memoryBlock);
        }

        T* operator->() const noexcept
        {
            return _data;
        }

        Ref& operator=(std::nullptr_t)
        {
            Release();
            _data = nullptr;
            _memoryBlock = nullptr;
            return *this;
        }

        Ref& operator=(const Ref& other) noexcept
        {
            Release();
            CopyConstructFrom(other);
            return *this;
        }

        template<typename TDerived> requires
            std::derived_from<TDerived, T> || std::same_as<T, void>
        Ref& operator=(const Ref<TDerived>& other) noexcept
        {
            Release();
            CopyConstructFrom(other);
            return *this;
        }

        Ref& operator=(Ref&& other) noexcept
        {
            Release();
            MoveConstructFrom(std::move(other));
            return *this;
        }

        template<typename TDerived> requires
            std::derived_from<TDerived, T> || std::same_as<T, void>
        Ref& operator=(Ref<TDerived>&& other) noexcept
        {
            Release();
            MoveConstructFrom(std::move(other));
            return *this;
        }

        explicit operator bool() const noexcept
        {
            return _memoryBlock != nullptr;
        }

        friend bool operator==(const Ref& left, const Ref& right) noexcept
        {
            return left._data == right._data;
        }
    private:
        explicit Ref(RefMemoryBlock<T>* memoryBlock):
            _memoryBlock(memoryBlock),
            _data(&memoryBlock->Value)
        {}

        void Release()
        {
            if(_memoryBlock && _memoryBlock->References.fetch_sub(1) == 1)
            {
                delete _memoryBlock;
                _memoryBlock = nullptr;
                _data = nullptr;
            }
        }

        template<typename TOther>
        void CopyConstructFrom(const Ref<TOther>& other) noexcept
        {
            _memoryBlock = other._memoryBlock;
            _data = other._data;

            if(_memoryBlock)
            {
                _memoryBlock->References++;
            }
        }

        template<typename TOther>
        void MoveConstructFrom(Ref<TOther>&& other) noexcept
        {
            _memoryBlock = other._memoryBlock;
            _data = other._data;

            other._memoryBlock = nullptr;
            other._data = nullptr;
        }

        template<typename TBase>
        void CopyConstructFromBase(const Ref<TBase>& base, T* data) noexcept
        {
            _memoryBlock = base._memoryBlock;
            _data = data;

            if(_memoryBlock)
            {
                _memoryBlock->References++;
            }
        }

        template<typename TBase>
        void MoveConstructFromBase(Ref<TBase>&& base, T* data) noexcept
        {
            _memoryBlock = base._memoryBlock;
            _data = data;

            base._memoryBlock = nullptr;
            base._data = nullptr;
        }

        template<typename TDerived>
        friend class Ref;
    private:
        RefMemoryBlockBase* _memoryBlock = nullptr;
        T* _data = nullptr;
    };

    /// <summary>
    /// Creates a new Ref<T> by constructing an object of type T with the given arguments.
    /// </summary>
    template<typename T, typename... TArgs>
    Ref<T> New(TArgs&&... args)
    {
        return Ref<T>(new RefMemoryBlock<T>(std::forward<TArgs>(args)...));
    }
}