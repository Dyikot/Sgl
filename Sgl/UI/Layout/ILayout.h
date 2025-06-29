#pragma once

namespace Sgl
{
    class ILayout
    {
    public:
        virtual ~ILayout() = default;

        virtual void QueryArrange() = 0;
    protected:
        virtual void Arrange() = 0;
    };
}