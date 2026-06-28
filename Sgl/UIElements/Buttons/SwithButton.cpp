#include "SwithButton.h"

namespace Sgl::UIElements
{
	SwitchButon::SwitchButon()
	{
		static ImageSource source(AssetId::SwitchButtonOff);

		SetWidth(48, ValueSource::Default);
		SetHeight(24, ValueSource::Default);
		SetBackground(source, ValueSource::Default);
	}
}

