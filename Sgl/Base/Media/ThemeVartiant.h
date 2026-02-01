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
    /// Event arguments provided when the effective theme variant changes.
    /// </summary>
    struct ThemeVariantChangedEventArgs
    {
        ThemeVariant Theme;
    };

    /// <summary>
    /// A container that holds separate values for light and dark theme modes.
    /// Used to declaratively define theme-aware resources (e.g., colors, brushes) that automatically adapt based on the current theme.
    /// </summary>
    template<typename T>
    struct ThemeResource
    {
        T LightValue;
        T DarkValue;
    };

    /// <summary>
    /// Retrieves the current effective theme mode of the application (Light or Dark),
    /// resolved based on the active ThemeVariant setting and, if applicable, the system preference.
    /// </summary>
    /// <returns>The current ThemeMode being used by the application.</returns>
    ThemeMode GetApplicationThemeMode();
}