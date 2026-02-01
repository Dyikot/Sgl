#pragma once

#include <string>
#include <string_view>

namespace Sgl
{
    /// <summary>
    /// Provides static methods to interact with the system clipboard for text operations.
    /// </summary>
    class Clipboard
    {
    public:
        /// <summary>
        /// Sets the system clipboard content to the specified text.
        /// Replaces any existing clipboard data.
        /// </summary>
        /// <param name="text"> - the text to place on the clipboard.</param>
        static void SetText(std::string_view text);

        /// <summary>
        /// Retrieves the current text content from the system clipboard.
        /// Returns an empty string if the clipboard is empty or does not contain text.
        /// </summary>
        /// <returns> - the text currently stored in the clipboard.</returns>
        static std::string GetText();

        /// <summary>
        /// Clears all content from the system clipboard.
        /// </summary>
        static void Clear();

        /// <summary>
        /// Checks whether the clipboard is empty or does not contain text data.
        /// </summary>
        /// <returns>True if the clipboard is empty; otherwise, false.</returns>
        static bool IsEmpty();
    };
}