#pragma once

#include <span>

namespace Sgl
{
	enum class AssetId
	{
		CheckBox,
		CheckBoxOutline,
		RadioButton,
		RadioButtonOutline
	};

	std::span<const uint8_t> LoadEmbeddedAsset(AssetId asset);
}