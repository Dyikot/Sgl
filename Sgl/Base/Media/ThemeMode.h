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

	/// <summary>
	/// Application theme resources
	/// </summary>
	struct ThemeResources;

	/// <summary>
	/// Get application theme resources. Must be implemented by user.
	/// </summary>
	/// <returns>Reference to application theme resources</returns>
	ThemeResources& GetThemeResources();
}