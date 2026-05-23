#pragma once

#include <unordered_map>
#include "LanguageInfo.h"
#include "../Tools/StringUtils.h"
#include "../Tools/CSVReader.h"

namespace Sgl
{
    /// <summary>
    /// Map type for storing localization key-value pairs.
    /// Uses std::string keys with custom StringHash hasher and transparent equality comparator
    /// to support efficient lookups with string_view without temporary string allocations.
    /// </summary>
    using LocalizationMap = std::unordered_map<std::string, std::string, StringHash, std::equal_to<void>>;

    /// <summary>
    /// Delegate type for loading localized strings.
    /// </summary>
    /// <param name="locale"> - the locale identifier (e.g., "en-US", "ru-RU").</param>
    /// <param name="languageInfo"> - reference to the current language information.</param>
    /// <returns>A map of localization keys to their translated strings.</returns>
    using LocalizationLoader = Func<LocalizationMap, const LanguageInfo&>;

    /// <summary>
    /// Manages localized strings and provides lookup functionality based on the current language.
    /// Loads localization data through a provided loader delegate and handles language change events.
    /// </summary>
    class LocalizationStorage final
    {        
    public:
        
        LocalizationStorage(LanguageManager& languageManager, LocalizationLoader localizationLoader);
        LocalizationStorage(const LocalizationStorage&) = delete;
        LocalizationStorage(LocalizationStorage&&) = default;
        ~LocalizationStorage();

        /// <summary>
        /// Retrieves the localized string for the specified key.
        /// </summary>
        /// <param name="key"> - the localization key to look up.</param>
        /// <returns>A localized string.</returns>
        std::string GetLocalizedString(std::string_view key) const;
    private:
        void OnLanguageChanged(LanguageManager& sender, EventArgs e);
        void LoadLocalizationStrings(const LanguageInfo& languageInfo);
    private:
        LocalizationMap _map;
        LanguageManager& _languageManager;
        LocalizationLoader _loader;
    };

    /// <summary>
    /// Specialized CSV reader for parsing localization files.
    /// Expects CSV format with the first column containing localization keys and subsequent columns 
    /// containing translations for different languages, with the first row containing column headers.
    /// Example format:
    ///   key,en-US,ru-RU,fr-FR
    ///   greeting,Hello,Привет,Bonjour
    /// </summary>
    class LocalizationCSVReader : public CSVReader
    {
    public:
        using CSVReader::CSVReader;

        /// <summary>
        /// Returns a localization map for the specified language.
        /// Assumes the first row contains column headers and the first column contains localization keys.
        /// </summary>
        /// <param name="languageName"> - the name of the language column to retrieve.</param>
        /// <returns>A map where keys are values from the first column and values are from the specified language column.</returns>
        LocalizationMap GetLocalization(std::string_view languageName) const;
    };
}