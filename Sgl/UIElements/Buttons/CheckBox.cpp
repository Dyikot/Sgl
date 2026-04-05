#include "CheckBox.h"
#include <SDL3/SDL_filesystem.h>

namespace Sgl::UIElements
{
	CheckBox::CheckBox()
	{
		static ImagePath imagePath = ImagePath(SDL_GetBasePath(), "Assets/Images/CheckButtonOutline.png");

		SetWidth(24, ValueSource::Default);
		SetHeight(24, ValueSource::Default);		
		SetBackground(imagePath, ValueSource::Default);
	}
}