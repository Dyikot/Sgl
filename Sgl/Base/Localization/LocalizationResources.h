#pragma once

#include <unordered_map>
#include "LanguageInfo.h"
#include "../Event.h"
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
    /// Delegate type for providing localized strings.
    /// </summary>
    /// <param name="language"> - the language identifier (e.g., "en", "ru", "fr").</param>
    /// <returns>A map of localization keys to their translated strings.</returns>
    using LocalizationProvider = Func<LocalizationMap, const LanguageInfo&>;

    /// <summary>
    /// Manages localized strings and provides lookup functionality based on the current language.
    /// Loads localization data through a provided delegate and handles language change events.
    /// </summary>
    class LocalizationResources
    {        
    public:
        using LanguageChangedEventHandler = EventHandler<LocalizationResources, const LanguageInfo&>;
    public:
        LocalizationResources() = default;
        LocalizationResources(const LocalizationResources&) = delete;
        LocalizationResources(LocalizationResources&&) = default;

        Event<LanguageChangedEventHandler> LanguageChanged;

        /// <summary>
        /// Initializes localization resources using the provider delegate.
        /// The provider delegate is invoked whenever the language changes to load the appropriate translations.
        /// </summary>
        /// <param name="localizationProvider"> - delegate that loads localization data for a specified locale and returns a map of localization keys to translated strings.</param>
        void SetProvider(LocalizationProvider localizationProvider);

        /// <summary>
        /// Initializes localization resources from CSV file.
        /// </summary>
        /// <param name="csvFilePath"> - path to the CSV file containing localization data.</param>
        /// <param name="delimiter"> - character used as delimiter in the CSV file (default is comma).</param>
        void SetCSVProvider(std::string csvFilePath, char delimiter = ',');

        void SetLanguage(const LanguageInfo& language);
        const LanguageInfo& GetLanguage() const { return _language; }

        /// <summary>
        /// Retrieves the localized string for the specified key.
        /// </summary>
        /// <param name="key"> - the localization key to look up.</param>
        /// <returns>A localized string.</returns>
        std::string GetLocalizedString(std::string_view key) const;
    private:
        void LoadLocalization();
    private:
        LocalizationMap _map;
        LanguageInfo _language { "en" };
        LocalizationProvider _provider;
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