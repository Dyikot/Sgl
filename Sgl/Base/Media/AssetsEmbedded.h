#pragma once

#include <span>

namespace Sgl
{
	enum class AssetId
	{
		CheckBox,
		CheckBoxOutline,
		RadioButton,
		RadioButtonOutline,
		SwitchButtonOff,
		SwitchButtonOn
	};

	std::span<const uint8_t> LoadEmbeddedAsset(AssetId asset);
}