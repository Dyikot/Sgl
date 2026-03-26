#pragma once

#include <string>
#include <unordered_map>
#include "Brush.h"
#include "ThemeMode.h"

namespace Sgl
{
    class ThemeResourceProvider
    {
    public:
        ThemeResourceProvider() = default;

        /// <summary>
        /// Adds a color resource for both light and dark themes.
        /// </summary>
        /// <param name="key"> - the resource key.</param>
        /// <param name="lightColor"> - the color value for light theme.</param>
        /// <param name="darkColor"> - the color value for dark theme.</param>
        void AddColor(const std::string& key, Color lightColor, Color darkColor);

        /// <summary>
        /// Adds a brush resource for both light and dark themes.
        /// </summary>
        /// <param name="key"> - the resource key.</param>
        /// <param name="lightBrush"> - the brush value for light theme.</param>
        /// <param name="darkBrush"> - the brush value for dark theme.</param>
        void AddBrush(const std::string& key, Brush lightBrush, Brush darkBrush);

        /// <summary>
        /// Gets a color resource for the current active theme.
        /// </summary>
        /// <param name="key"> - the resource key.</param>
        /// <returns>The color value.</returns>
        /// <exception cref="std::runtime_error">Thrown when the key is not found.</exception>
        Color GetColor(const std::string& key) const;

        /// <summary>
        /// Gets a brush resource for the current active theme.
        /// </summary>
        /// <param name="key"> - the resource key.</param>
        /// <returns>The brush value.</returns>
        /// <exception cref="std::runtime_error">Thrown when the key is not found.</exception>
        Brush GetBrush(const std::string& key) const;

        /// <summary>
        /// Tries to get a color resource for the current active theme.
        /// </summary>
        /// <param name="key"> - the resource key.</param>
        /// <param name="out"> - output parameter for the color value.</param>
        /// <returns>True if the key was found, false otherwise.</returns>
        bool TryGetColor(const std::string& key, Color& out) const;

        /// <summary>
        /// Tries to get a brush resource for the current active theme.
        /// </summary>
        /// <param name="key"> - the resource key.</param>
        /// <param name="out"> - output parameter for the brush value.</param>
        /// <returns>True if the key was found, false otherwise.</returns>
        bool TryGetBrush(const std::string& key, Brush& out) const;

        /// <summary>
        /// Sets the current active theme mode.
        /// </summary>
        /// <param name="mode"> - the new theme mode.</param>
        void SetCurrentTheme(ThemeMode mode);

        /// <summary>
        /// Gets the current active theme mode.
        /// </summary>
        ThemeMode GetCurrentTheme() const { return _currentMode; }
    private:
        struct ThemeDictionary
        {
            std::unordered_map<std::string, Color> Colors;
            std::unordered_map<std::string, Brush> Brushes;
        };

        ThemeDictionary _resources[2];
        ThemeMode _currentMode = ThemeMode::Light;
    };
}
