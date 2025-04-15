#pragma once

#include "SDL/SDL_clipboard.h"
#include <string_view>
#include <string>

namespace Sgl
{
	class Clipboard
	{
	public:
		static void SetText(std::string_view text)
		{
			SDL_SetClipboardText(text.data());
		}

		static auto GetText()
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