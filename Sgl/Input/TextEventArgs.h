#pragma once

#include <string>

namespace Sgl
{
    //! @brief Event arguments for in-progress text composition.
    //! Represents the current intermediate text being edited, along with the cursor/selection range
    struct TextEditingEventArgs
    {
        std::string Text;
        int SelectionStart;
        int SelectionLength;
    };

    //! @brief Event arguments for committed text input, typically triggered when the user finalizes input
    struct TextInputEventArgs
    {
        std::string Text;
    };
}