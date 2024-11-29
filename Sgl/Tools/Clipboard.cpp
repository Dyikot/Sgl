#include "Clipboard.h"

namespace Sgl
{
    std::string Clipboard::GetText()
    {
        auto tempText = SDL_GetClipboardText();
        std::string text = tempText;
        SDL_free(tempText);
        return text;
    }
}
