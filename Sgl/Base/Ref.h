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
    struct RefMemoryBlock: RefMemoryBlockBase
    {
        T Value;

        template<typename... TArgs>
        RefMemoryBlock(TArgs&&... args)
            : Value(std::forward<TArgs>(args)...)
        {}
    };

    template<typename T>
    class Ref
    {
    private:
        RefMemoryBlockBase* _memoryBlock = nullptr;
        T* _value = nullptr;
    public:
        Ref() noexcept = default;
        Ref(std::nullptr_t) noexcept {}

        template<typename... TArgs>
        Ref(std::in_place_t, TArgs&&... args):
            _memoryBlock(new RefMemoryBlock<T>(std::forward<TArgs>(args)... )),
            _value(&static_cast<RefMemoryBlock<T>*>(_memoryBlock)->Value)
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

        ~Ref()
        {
            TryDeleteMemoryBlock();            
        }

        template<typename T, typename... TArgs>
        friend Ref<T> New(TArgs&&... args);

        T* Get() const noexcept
        {
            return _value;
        }

        template<typename TOther>
        TOther* GetAs() const
        {
            return static_cast<TOther*>(_value);
        }

        template<std::derived_from<T> TOther>
        TOther* TryGetAs() const
        {
            return dynamic_cast<TOther*>(_value);
        }
        
        template<typename TValue = T> requires !std::is_void_v<TValue>
        TValue& GetValue() const
        {
            return *_value;
        }

        template<typename TValue> requires !std::is_void_v<TValue>
        TValue& GetValueAs() const
        {
            return static_cast<TValue&>(*_value);
        }

        T* operator->() const
        {
            return _value;
        }

        Ref& operator=(const Ref& other) noexcept
        {
            if(this != &other)
            {
                TryDeleteMemoryBlock();
                CopyConstructFrom(other);
            }            

            return *this;
        }

        template<typename TDerived> requires
            std::derived_from<TDerived, T> || std::same_as<T, void>
        Ref& operator=(const Ref<TDerived>& other) noexcept
        {
            if(this != &other)
            {
                TryDeleteMemoryBlock();
                CopyConstructFrom(other);
            }

            return *this;
        }

        Ref& operator=(Ref&& other) noexcept
        {
            if(this != &other)
            {
                TryDeleteMemoryBlock();
                MoveConstructFrom(std::move(other));
            }

            return *this;
        }

        template<typename TDerived> requires
            std::derived_from<TDerived, T> || std::same_as<T, void>
        Ref& operator=(Ref<TDerived>&& other) noexcept
        {
            if(this != &other)
            {
                TryDeleteMemoryBlock();
                MoveConstructFrom(std::move(other));
            }

            return *this;
        }

        explicit operator bool() const noexcept
        {
            return _memoryBlock != nullptr;
        }

        friend bool operator==(const Ref& left, const Ref& right)
        {
            return left._value == right._value;
        }
    private:
        explicit Ref(RefMemoryBlock<T>* memoryBlock):
            _memoryBlock(memoryBlock),
            _value(&memoryBlock->Value)
        {}

        void TryDeleteMemoryBlock()
        {
            if(_memoryBlock && _memoryBlock->References.fetch_sub(1) == 1)
            {
                delete _memoryBlock;
            }
        }

        template<typename TOther>
        void CopyConstructFrom(const Ref<TOther>& other) noexcept
        {
            _memoryBlock = other._memoryBlock;
            _value = other._value;

            if(_memoryBlock)
            {
                _memoryBlock->References++;
            }
        }

        template<typename TOther>
        void MoveConstructFrom(Ref<TOther>&& other) noexcept
        {
            _memoryBlock = other._memoryBlock;
            _value = other._value;

            other._memoryBlock = nullptr;
            other._value = nullptr;
        }

        template<typename TDerived>
        friend class Ref;
    };

    template<typename T, typename... TArgs>
    Ref<T> New(TArgs&&... args)
    {
        return Ref<T>(new RefMemoryBlock<T>(std::forward<TArgs>(args)...));
    }

    template<typename T, typename TConfigurer>
    Ref<T> CreateWith(TConfigurer&& configurer)
    {
        auto element = New<T>();
        configurer(element.GetValue());
        return element;
    }
}