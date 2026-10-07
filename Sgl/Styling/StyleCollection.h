#pragma once

#include "Style.h"

namespace Sgl
{
    //! @brief A collection of styles that can be applied to UI elements
    class StyleCollection : public Vector<Style>
    {
    public:
        using Vector::Vector;
        StyleCollection(const StyleCollection&) = delete;
        StyleCollection(StyleCollection&&) noexcept = default;

        void MergeStylesTo(Styleable& element, Style& target)
        {
            for(auto& style : _data)
            {
                if(style.selector.Match(element))
                {
                    target.Merge(style);
                }
            }
        }

        StyleCollection& operator=(const StyleCollection&) = delete;
        StyleCollection& operator=(StyleCollection&&) noexcept = default;
    };
}