#pragma once

#include <string_view>
#include <span>

namespace Sgl
{
	enum class AssetId
	{
		CheckButton,
		CheckButtonOutline,
		RadioButton,
		RadioButtonOutline
	};

	std::span<const uint8_t> LoadAssetRaw(AssetId asset);
}