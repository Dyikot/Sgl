#pragma once

#include <concepts>
#include "../Base/Delegate.h"

namespace Sgl
{
    class Styleable;

    //! @brief Type-erased selector function
    using TargetSelector = Func<Styleable&, Styleable&>;

    //! @brief Concept kept to validate selectors when they are added to the chain
    template<typename T>
    concept CTargetSelector = requires(T func, Styleable & target)
    {
        { func(target) } -> std::same_as<Styleable&>;
    };

    //! @brief Composed selector: applies an ordered chain of selectors
    class ComposedTargetSelector
    {
    public:
        ComposedTargetSelector() = default;
        
        explicit ComposedTargetSelector(std::vector<TargetSelector> selectors):
            _selectors(std::move(selectors))
        {}

        //! @brief Appends a selector
        void Append(TargetSelector selector)
        {
            _selectors.emplace_back(std::move(selector));
        }

        //! @brief Concatenates another chain
        void Append(ComposedTargetSelector other)
        {
            _selectors.insert(_selectors.end(),
                              std::make_move_iterator(other._selectors.begin()),
                              std::make_move_iterator(other._selectors.end()));
        }

        //! @brief Inserts a selector
        void Prepend(TargetSelector selector)
        {
            _selectors.emplace(_selectors.begin(), std::move(selector));
        }

        //! @brief Applies the chain in order; an empty chain is the identity
        Styleable& operator()(Styleable& target) const
        {
            Styleable* current = &target;
            for(const auto& selector : _selectors)
            {
                current = &selector(*current);
            }

            return *current;
        }

    private:
        std::vector<TargetSelector> _selectors;
    };

    inline ComposedTargetSelector operator>(TargetSelector left, TargetSelector right)
    {
        return ComposedTargetSelector({ std::move(left), std::move(right) });
    }

    inline ComposedTargetSelector operator>(ComposedTargetSelector left, TargetSelector right)
    {
        left.Append(std::move(right));
        return left;
    }

    inline ComposedTargetSelector operator>(TargetSelector left, ComposedTargetSelector right)
    {
        right.Prepend(std::move(left));
        return right;
    }

    inline ComposedTargetSelector operator>(ComposedTargetSelector left, ComposedTargetSelector right)
    {
        left.Append(std::move(right));
        return left;
    }
}