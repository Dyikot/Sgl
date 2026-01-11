#pragma once

namespace Sgl
{
	enum class ThemeMode
	{
		Light, Dark
	};

	enum class ThemeVariant
	{
		Light, Dark, System
	};

	struct ThemeVariantChangedEventArgs
	{
		ThemeVariant Theme;
	};

	template<typename T>
	struct ThemeResource
	{
		T LightValue;
		T DarkValue;
	};

	ThemeMode GetApplicationThemeMode();
}