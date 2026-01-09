#include "Clipboard.h"
#include "../Base/Logger.h"
#include <SDL3/SDL_clipboard.h>

namespace Sgl
{
	void Clipboard::SetText(std::string_view text)
	{
		if(!SDL_SetClipboardText(text.data()))
		{
			Logger::LogWarning("Unable to set a text to clipboard: {}", SDL_GetError());
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
			Logger::LogWarning("Unable to clear clipboard: {}", SDL_GetError());
		}
	}

	bool Clipboard::IsEmpty()
	{
		return SDL_HasClipboardText();
	}
}