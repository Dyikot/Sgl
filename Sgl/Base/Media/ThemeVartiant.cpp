#include "ThemeVartiant.h"
#include "../../Application.h"

namespace Sgl
{
    ThemeMode Sgl::GetApplicationThemeMode()
    {
        return App->GetThemeMode();
    }
}
