#pragma once

#include <string>
#include <string_view>

namespace Sgl
{
	//! @brief Provides static methods to interact with the system clipboard for text operations
	class Clipboard
	{
	public:
		//! @brief Sets the system clipboard content to the specified text. Replaces any existing clipboard data.
		//! @param text The text to place on the clipboard
		static void SetText(std::string_view text);

		//! @brief Retrieves the current text content from the system clipboard. Returns an empty string if the clipboard is empty or does not contain text.
		//! @return The text currently stored in the clipboard
		static std::string GetText();

		//! @brief Clears all content from the system clipboard
		static void Clear();

		//! @brief Checks whether the clipboard is empty or does not contain text data
		//! @return True if the clipboard is empty; otherwise, false
		static bool IsEmpty();
	};
}