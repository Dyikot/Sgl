#pragma once

#include <string>

namespace Sgl
{
    /// <summary>
    /// Represents a strongly-typed key for accessing theme resources.
    /// Provides type safety when referencing resources in styles.
    /// </summary>
    struct ResourceKey
    {
        std::string Value;
    };
}