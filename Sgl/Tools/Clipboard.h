#pragma once
#include "SDL/SDL_clipboard.h"
#include <string_view>

namespace Sgl
{
	class Clipboard
	{
	public:
		static void SetText(std::string_view text) { SDL_SetClipboardText(text.data()); }
		static void Clear() { SetText(""); }
		static std::string GetText();
		static bool ContainsData() { SDL_HasClipboardText(); }
	};
}