#pragma once

#include <string>

namespace Sgl
{
    class ResourceKey
    {
    public:
        explicit ResourceKey(std::string name):
            _name(std::move(name))
        {}

        const std::string& GetName() const { return _name; }
    private:
        std::string _name;
    };
}