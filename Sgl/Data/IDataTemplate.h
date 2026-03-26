#pragma once

#include "../Base/Ref.h"

namespace Sgl
{
    class UIElement;
    class INotifyPropertyChanged;

    /// <summary>
    /// Defines a template for creating UI elements based on data objects.
    /// Used in data binding scenarios to generate visual representations of data.
    /// </summary>
    class IDataTemplate
    {
    public:
        virtual ~IDataTemplate() = default;

        /// <summary>
        /// Builds a UI element for the specified data object.
        /// </summary>
        /// <param name="data"> - the data object to create a UI for.</param>
        /// <returns>A reference to the created UI element.</returns>
        virtual Ref<UIElement> Build(const Ref<INotifyPropertyChanged>& data) = 0;

        /// <summary>
        /// Determines whether this template matches the specified data object.
        /// </summary>
        /// <param name="data"> - the data object to check.</param>
        /// <returns>True if the template can handle this data, false otherwise.</returns>
        virtual bool Match(const Ref<INotifyPropertyChanged>& data) const = 0;
    };
}