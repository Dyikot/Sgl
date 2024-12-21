#include "Clipboard.h"

namespace Sgl
{
    std::string Clipboard::GetText()
    {
        auto rawText = SDL_GetClipboardText();
        std::string text = rawText;
        SDL_free(rawText);
        return text;
    }
}
