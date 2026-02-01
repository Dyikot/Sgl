#pragma once

#include <string>

namespace Sgl
{
    /// <summary>
    /// Event arguments for in-progress text composition.
    /// Represents the current intermediate text being edited, along with the cursor/selection range.
    /// </summary>
    struct TextEditingEventArgs
    {
        std::string Text;
        int SelectionStart;
        int SelectionLength;
    };

    /// <summary>
    /// Event arguments for committed text input, typically triggered when the user finalizes input.
    /// </summary>
    struct TextInputEventArgs
    {
        std::string Text;
    };
}