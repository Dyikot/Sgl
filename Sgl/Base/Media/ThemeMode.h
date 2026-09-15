#pragma once

namespace Sgl
{
	//! @brief Specifies the visual theme mode used for rendering UI elements
	enum class ThemeMode
	{
		Light,
		Dark
	};

	//! @brief Specifies how the application should determine its active theme
	enum class ThemeVariant
	{
		Light,
		Dark,
		System
	};
}