#pragma once

#include "SDL/SDL_clipboard.h"
#include <string_view>
#include <string>

namespace Sgl
{
	class Clipboard
	{
	public:
		static void SetText(const std::string& text)
		{
			SDL_SetClipboardText(text.data());
		}

		static std::string GetText()
		{
			auto rawText = SDL_GetClipboardText();
			std::string text = rawText;
			SDL_free(rawText);
			return text;
		}

		static void Clear()
		{ 
			SetText(""); 
		}

		static bool IsEmpty()
		{
			return SDL_HasClipboardText() == SDL_FALSE;
		}
	};
}