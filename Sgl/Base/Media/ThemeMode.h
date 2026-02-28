#pragma once

namespace Sgl
{
	/// <summary>
	/// Specifies the visual theme mode used for rendering UI elements.
	/// </summary>
	enum class ThemeMode
	{
		Light,
		Dark
	};

	/// <summary>
	/// Specifies how the application should determine its active theme.
	/// </summary>
	enum class ThemeVariant
	{
		Light,
		Dark,
		System
	};
}