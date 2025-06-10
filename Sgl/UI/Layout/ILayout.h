#pragma once

namespace Sgl::UI
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