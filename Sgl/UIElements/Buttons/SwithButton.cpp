#include "SwithButton.h"

namespace Sgl::UIElements
{
	SwitchButon::SwitchButon()
	{
		static ImageSource source(AssetId::SwitchButtonOff);

		SetName("SwitchButon");
		SetWidth(48, ValueSource::Default);
		SetHeight(24, ValueSource::Default);
		SetBackground(source, ValueSource::Default);
	}
}

