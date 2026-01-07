#pragma once

#include <atomic>
#include <memory>
#include <utility>
#include <stdexcept>

namespace Sgl
{
    struct RefMemoryBlockBase
    {
        std::atomic<size_t> References = 1;
        virtual ~RefMemoryBlockBase() = default;
    };

    template<typename T>
    struct RefMemoryBlock : RefMemoryBlockBase
    {
        T Value;

        template<typename... TArgs>
        RefMemoryBlock(TArgs&&... args)
            : Value(std::forward<TArgs>(args)...)
        {}
    };

    template<typename T>
    class Ref final
    {
    private:
        RefMemoryBlockBase* _memoryBlock = nullptr;
        T* _data = nullptr;
    public:
        Ref() noexcept = default;
        Ref(std::nullptr_t) noexcept {}

        template<typename... TArgs>
        Ref(std::in_place_t, TArgs&&... args):
            _memoryBlock(new RefMemoryBlock<T>(std::forward<TArgs>(args)... )),
            _data(&static_cast<RefMemoryBlock<T>*>(_memoryBlock)->Value)
        {}

        Ref(const Ref& other) noexcept
        {
            CopyConstructFrom(other);
        }

        template<typename TDerived> requires
            std::derived_from<TDerived, T> || std::same_as<T, void>
        Ref(const Ref<TDerived>& other) noexcept
        {
            CopyConstructFrom(other);
        }

        Ref(Ref&& other) noexcept
        {
            MoveConstructFrom(std::move(other));
        }

        template<typename TDerived> requires
            std::derived_from<TDerived, T> || std::same_as<T, void>
        Ref(Ref<TDerived>&& other) noexcept
        {
            MoveConstructFrom(std::move(other));
        }

        template<typename TBase> requires std::is_base_of_v<TBase, T>
        Ref(const Ref<TBase>& base, T* data) noexcept
        {
            CopyConstructFromBase(base, data);
        }

        template<typename TBase> requires std::is_base_of_v<TBase, T>
        Ref(Ref<TBase>&& base, T* data) noexcept
        {
            MoveConstructFromBase(std::move(base), data);
        }

        ~Ref()
        {
            Release();            
        }

        template<typename T, typename... TArgs>
        friend Ref<T> New(TArgs&&... args);

        template<typename TDerived> 
        Ref<TDerived> As() const
        {
            return Ref<TDerived>(*this, GetAs<TDerived>());
        }

        template<std::derived_from<T> TDerived>
        bool Is() const
        {
            return dynamic_cast<TDerived*>(_data);
        }

        T* Get() const noexcept
        {
            return _data;
        }

        template<typename TOther>
        TOther* GetAs() const
        {
            return static_cast<TOther*>(_data);
        }
        
        template<typename TValue = T> requires !std::is_void_v<TValue>
        TValue& GetValue() const
        {
            return *_data;
        }

        template<typename TValue> requires !std::is_void_v<TValue>
        TValue& GetValueAs() const
        {
            return static_cast<TValue&>(*_data);
        }

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
    };

    template<typename T, typename... TArgs>
    Ref<T> New(TArgs&&... args)
    {
        return Ref<T>(new RefMemoryBlock<T>(std::forward<TArgs>(args)...));
    }
}