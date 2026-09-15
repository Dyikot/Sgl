#pragma once

#include "../Base/Ref.h"

namespace Sgl
{
    class UIElement;
    class ObservableObject;

    //! @brief Defines a template for creating UI elements based on data objects
    class IDataTemplate : public RefCounted
    {
    public:
        //! @brief Builds a UI element for the specified data object
        //! @param data The data object to create a UI for
        //! @return A reference to the created UI element
        virtual Ref<UIElement> Build(const Ref<ObservableObject>& data) = 0;

        //! @brief Determines whether this template matches the specified data object
        //! @param data The data object to check
        //! @return True if the template can handle this data, false otherwise
        virtual bool Match(const Ref<ObservableObject>& data) const = 0;
    };
}