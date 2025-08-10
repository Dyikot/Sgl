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
}