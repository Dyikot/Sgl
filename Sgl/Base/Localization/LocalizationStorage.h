#pragma once

#include <unordered_map>
#include "LanguageInfo.h"

namespace Sgl
{
    /// <summary>
    /// Delegate type for loading localized strings.
    /// </summary>
    /// <param name="locale"> - the locale identifier (e.g., "en-US", "ru-RU").</param>
    /// <param name="languageInfo"> - reference to the current language information.</param>
    /// <returns>A map of localization keys to their translated strings.</returns>
    using LocalizationLoader = Func<std::unordered_map<std::string, std::string>, const LanguageInfo&>;

    /// <summary>
    /// Manages localized strings and provides lookup functionality based on the current language.
    /// Loads localization data through a provided loader delegate and handles language change events.
    /// </summary>
    class LocalizationStorage final
    {        
    public:
        /// <summary>
        /// Constructs a LocalizationStorage instance with the specified loader.
        /// </summary>
        /// <param name="localizationLoader">Delegate used to load localized strings for a given locale.</param>
        LocalizationStorage(LocalizationLoader localizationLoader);
        LocalizationStorage(const LocalizationStorage&) = delete;
        LocalizationStorage(LocalizationStorage&&) = default;
        ~LocalizationStorage();

        /// <summary>
        /// Retrieves the localized string for the specified key.
        /// </summary>
        /// <param name="key"> - the localization key to look up.</param>
        /// <returns>A localized string.</returns>
        std::string GetLocalizedString(const std::string& key) const;
    private:
        void OnLanguageChanged(LanguageInfo& sender, EventArgs e);
        void LoadLocalizationStrings(const LanguageInfo& languageInfo);
    private:
        std::unordered_map<std::string, std::string> _localizedStrings;
        LocalizationLoader _localizationLoader;
    };
}