#include "CheckBox.h"

namespace Sgl::UIElements
{
	CheckBox::CheckBox()
	{
		static ImageSource source(AssetId::CheckBoxOutline);

		SetWidth(24, ValueSource::Default);
		SetHeight(24, ValueSource::Default);		
		SetBackground(source, ValueSource::Default);
	}
}