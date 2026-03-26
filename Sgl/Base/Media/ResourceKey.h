#pragma once

#include <string>

namespace Sgl
{
    /// <summary>
    /// Represents a strongly-typed key for accessing theme resources.
    /// Provides type safety when referencing resources in styles.
    /// </summary>
    class ResourceKey
    {
    public:
        /// <summary>
        /// Initializes a new instance of the ResourceKey class with the specified name.
        /// </summary>
        /// <param name="name"> - the resource name.</param>
        explicit ResourceKey(std::string name):
            _name(std::move(name))
        {}

        /// <summary>
        /// Gets the resource key name.
        /// </summary>
        const std::string& GetName() const { return _name; }
    private:
        std::string _name;
    };
}