#include "CheckBox.h"

namespace Sgl::UIElements
{
	CheckBox::CheckBox()
	{
		static ImageSource imagePath(AssetId::CheckButtonOutline);

		SetWidth(24, ValueSource::Default);
		SetHeight(24, ValueSource::Default);		
		SetBackground(imagePath, ValueSource::Default);
	}
}