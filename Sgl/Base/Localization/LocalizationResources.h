#pragma once

#include <unordered_map>

#include "LanguageInfo.h"
#include "../Event.h"
#include "../Tools/StringUtils.h"
#include "../Tools/CSVReader.h"

namespace Sgl
{
    //! @brief Map type for storing localization key-value pairs.
    //! Uses std::string keys with custom StringHash hasher and transparent equality comparator 
    //! to support efficient lookups with string_view without temporary string allocations.
    using LocalizationMap = std::unordered_map<std::string, std::string, StringHash, std::equal_to<>>;

    //! @brief Delegate type for providing localized strings
    using LocalizationProvider = Func<LocalizationMap, const LanguageInfo&>;

    //! @brief Manages localized strings and provides lookup functionality based on the current language.
    //! Loads localization data through a provided delegate and handles language change events.
    class LocalizationResources
    {        
    public:
        using LanguageChangedEventHandler = EventHandler<LocalizationResources, const LanguageInfo&>;
    public:
        LocalizationResources() = default;
        LocalizationResources(const LocalizationResources&) = delete;
        LocalizationResources(LocalizationResources&&) = default;

        //! @brief Event triggered immediately after the current language has been successfully changed
        Event<LanguageChangedEventHandler> LanguageChanged;

        //! @brief Initializes localization resources using the provider delegate.
        //! @param localizationProvider Delegate that loads localization data for a specified locale and returns a map of localization keys to translated strings
        void SetProvider(LocalizationProvider localizationProvider);

        //! @brief Initializes localization resources from CSV file
        //! @param csvFilePath Path to the CSV file containing localization data
        //! @param delimiter Character used as delimiter in the CSV file
        void SetCSVProvider(std::string csvFilePath, char delimiter = ',');

        //! @brief Changes the current language and automatically reloads the localization data
        //! @param language Information about the target language/locale to switch to
        void SetLanguage(const LanguageInfo& language);

        //! @brief Gets the currently active language information
        //! @return A constant reference to the LanguageInfo object
        const LanguageInfo& GetLanguage() const { return _language; }

        //! @brief Retrieves the localized string for the specified key
        //! @param key The localization key to look up
        //! @return A localized string
        std::string GetLocalizedString(std::string_view key) const;
    private:
        void LoadLocalization();
    private:
        LocalizationMap _map;
        LanguageInfo _language { "en" };
        LocalizationProvider _provider;
    };
    
    //! @brief Specialized CSV reader for parsing localization files.
    //! 
    //! Expects CSV format with the first column containing localization keys and subsequent columns 
    //! containing translations for different languages, with the first row containing column headers.
    //! 
    //! Example format:
    //!   key,en-US,ru-RU,fr-FR
    //!   greeting,Hello,Привет,Bonjour
    class LocalizationCSVReader : public CSVReader
    {
    public:
        using CSVReader::CSVReader;

        //! @brief Returns a localization map for the specified language.
        //! 
        //! Assumes the first row contains column headers and the first column contains localization keys.
        //! @param languageName The name of the language column to retrieve
        //! @return A map where keys are values from the first column and values are from the specified language column
        LocalizationMap GetLocalization(std::string_view languageName) const;
    };
}