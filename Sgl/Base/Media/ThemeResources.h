#pragma once

#include <string>
#include <unordered_map>

#include "Brush.h"
#include "ThemeMode.h"

namespace Sgl
{
	//! @brief Stores themed color and brush resources that automatically switch based on the active theme.
	//! Resources are stored separately for light and dark themes and are looked up at runtime.
	class ThemeResources
	{
	public:
		ThemeResources() = default;
		~ThemeResources();

		//! @brief Adds a color resource for both light and dark themes
		//! @param key The resource key
		//! @param lightColor The color value for light theme
		//! @param darkColor The color value for dark theme
		void AddColor(const std::string& key, Color lightColor, Color darkColor);

		//! @brief Adds a brush resource for both light and dark themes
		//! @param key The resource key
		//! @param lightBrush The brush value for light theme
		//! @param darkBrush The brush value for dark theme
		void AddBrush(const std::string& key, const Brush& lightBrush, const Brush& darkBrush);

		//! @brief Gets a color resource for the current active theme
		//! @param key The resource key
		//! @return The color value
		Color GetColor(const std::string& key) const;

		//! @brief Gets a brush resource for the current active theme
		//! @param key The resource key
		//! @return The brush value
		Brush GetBrush(const std::string& key) const;

		//! @brief Tries to get a color resource for the current active theme
		//! @param key The resource key
		//! @param out Output parameter for the color value
		//! @return True if the key was found, false otherwise
		bool TryGetColor(const std::string& key, Color& out) const;

		//! @brief Tries to get a brush resource for the current active theme
		//! @param key The resource key
		//! @param out Output parameter for the brush value
		//! @return True if the key was found, false otherwise
		bool TryGetBrush(const std::string& key, Brush& out) const;

		//! @brief Sets the current active theme mode
		//! @param mode The new theme mode
		void SetCurrentTheme(ThemeMode mode);

		//! @brief Gets the current active theme mode
		ThemeMode GetCurrentTheme() const { return _currentMode; }

	private:
		struct ThemeDictionary
		{
			std::unordered_map<std::string, Color> Colors;
			std::unordered_map<std::string, Brush> Brushes;
		};

		ThemeMode _currentMode = ThemeMode::Light;
		ThemeDictionary* _resources = new ThemeDictionary[2];
	};
}