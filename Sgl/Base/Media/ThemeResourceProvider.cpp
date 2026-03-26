#include "ThemeResourceProvider.h"
#include "../Exceptions.h"

namespace Sgl
{
    static constexpr size_t LightIndex = static_cast<size_t>(ThemeMode::Light);
    static constexpr size_t DarkIndex = static_cast<size_t>(ThemeMode::Dark);

    void ThemeResourceProvider::AddColor(const std::string& key, Color lightColor, Color darkColor)
    {
        _resources[LightIndex].Colors.emplace(key, lightColor);
        _resources[DarkIndex].Colors.emplace(key, darkColor);
    }

    void ThemeResourceProvider::AddBrush(const std::string& key, Brush lightBrush, Brush darkBrush)
    {
        _resources[LightIndex].Brushes.emplace(key, std::move(lightBrush));
        _resources[DarkIndex].Brushes.emplace(key, std::move(darkBrush));
    }

    Color ThemeResourceProvider::GetColor(const std::string& key) const
    {
        Color result = Colors::Transparent;

        if (!TryGetColor(key, result))
        {
            throw Exception("Resource color '{}' not found for current theme", key);
        }

        return result;
    }

    Brush ThemeResourceProvider::GetBrush(const std::string& key) const
    {
        Brush result = Colors::Transparent;

        if (!TryGetBrush(key, result))
        {
            throw Exception("Resource brush '{}' not found for current theme", key);
        }

        return result;
    }

    bool ThemeResourceProvider::TryGetColor(const std::string& key, Color& out) const
    {
        const auto& dict = _resources[static_cast<size_t>(_currentMode)];
        
        if (auto it = dict.Colors.find(key); it != dict.Colors.end())
        {
            out = it->second;
            return true;
        }

        return false;
    }

    bool ThemeResourceProvider::TryGetBrush(const std::string& key, Brush& out) const
    {
        const auto& dict = _resources[static_cast<size_t>(_currentMode)];
        
        if (auto it = dict.Brushes.find(key); it != dict.Brushes.end())
        {
            out = it->second;
            return true;
        }

        return false;
    }

    void ThemeResourceProvider::SetCurrentTheme(ThemeMode mode)
    {
        _currentMode = mode;
    }
}
