#pragma once

#include "Styleable.h"
#include "../Base/Media/Brush.h"

namespace Sgl
{
    //! @brief A context class responsible for applying styles to a Styleable target object
    class StyleContext
    {
    public:
        //! @brief Constructs a StyleContext bound to a specific target
        //! @param target The styleable object that properties will be applied to
        explicit StyleContext(Styleable& target);

        //! @brief Sets a direct value for a given styleable property
        //! @param property The styleable property instance
        //! @param value The value to assign to the property
        template<typename TOwner, typename TValue>
        void Set(StyleableProperty<TOwner, TValue>& property, typename StyleableProperty<TOwner, TValue>::Value value)
        {
            property.InvokeSetter(static_cast<TOwner&>(_target), value);
        }

        //! @brief Sets a Color value on a property by looking up a resource key
        //! @param property The styleable property instance expecting a Color
        //! @param key The resource key used to look up the Color
        template<typename TOwner>
        void Set(StyleableProperty<TOwner, Color>& property, const std::string& key)
        {
            property.InvokeSetter(static_cast<TOwner&>(_target), GetColor(key));
        }

        //! @brief Sets a Brush value on a property by looking up a resource key
        //! @param property The styleable property instance expecting a const Brush&
        //! @param key The resource key used to look up the Brush
        template<typename TOwner>
        void Set(StyleableProperty<TOwner, const Brush&>& property, const std::string& key)
        {
            property.InvokeSetter(static_cast<TOwner&>(_target), GetBrush(key));
        }

    private:
        //! @brief Retrieves a Color from resources using its identifier key
        //! @param key The resource key of the color
        //! @return The resolved Color object
        Color GetColor(const std::string& key) const;

        //! @brief Retrieves a Brush from resources using its identifier key
        //! @param key The resource key of the brush
        //! @return The resolved Brush object
        Brush GetBrush(const std::string& key) const;

    private:
        Styleable& _target;
    };
}