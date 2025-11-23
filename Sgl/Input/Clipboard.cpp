#include "Clipboard.h"
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_clipboard.h>

namespace Sgl
{
	void Clipboard::SetText(std::string_view text)
	{
		if(!SDL_SetClipboardText(text.data()))
		{
			SDL_Log("Unable to set a text to clipboard: %s", SDL_GetError());
		}
	}

	std::string Clipboard::GetText()
	{
		auto rawText = SDL_GetClipboardText();
		std::string text = rawText;
		SDL_free(rawText);
		return text;
	}

	void Clipboard::Clear()
	{
		if(!SDL_ClearClipboardData())
		{
			SDL_Log("Unable to clear clipboard: %s", SDL_GetError());
		}
	}

	bool Clipboard::IsEmpty()
	{
		return SDL_HasClipboardText();
	}
}