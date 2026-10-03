#pragma once

#include <unordered_map>

#include "../Delegate.h"
#include "../Tools/StringUtils.h"
#include "LanguageInfo.h"

namespace Sgl
{
    //! @brief Map type for storing localization key-value pairs.
    //! Uses std::string keys with custom StringHash hasher and transparent equality comparator 
    //! to support efficient lookups with string_view without temporary string allocations.
    using LocalizationMap = std::unordered_map<std::string, std::string, StringHash, std::equal_to<>>;

    //! @brief Delegate type for providing localized strings
    using LocalizationProvider = Func<LocalizationMap, const LanguageInfo&>;

    //! @brief A in-memory localization provider that generates localization maps on demand
    class InMemoryLocalizationProvider
    {
    public:
        //! @brief Constructs an empty provider
        InMemoryLocalizationProvider() = default;

        //! @brief Constructs a provider with an initial set of language factories
        //! @param factories A map of language codes to their respective map factory functions
        explicit InMemoryLocalizationProvider(std::unordered_map<LanguageInfo, Func<LocalizationMap>> factories);

        //! @brief Retrieves the localization map for the specified language
        //! @param language The target language
        //! @return A LocalizationMap containing the localized key-value pairs for the requested language
        LocalizationMap operator()(const LanguageInfo& language) const;

        //! @brief Registers a new language factory
        //! @param language The language info
        //! @param factory A callable that returns a LocalizationMap when invoked
        void AddLanguage(LanguageInfo languange, Func<LocalizationMap> factory);

        //! @brief Checks if a factory for a specific language is registered
        //! @param language The language info to check
        //! @return true if a factory exists, false otherwise
        bool HasLanguage(const LanguageInfo& language) const;

    private:
        std::unordered_map<LanguageInfo, Func<LocalizationMap>> _factories;
    };

    //! @brief A localization provider that loads localized strings from a CSV file
    class CSVLocalizationProvider
    {
    public:
        //! @brief Constructor
        //! @param filePath The path to the CSV file containing the localization data
        //! @param delimeter The character used to separate fields in the CSV file
        CSVLocalizationProvider(std::string filePath, char delimeter = ',');
        
        //! @brief Retrieves the localization map for the specified language
        //! @param language The target language
        //! @return A LocalizationMap containing the localized key-value pairs for the requested language
        LocalizationMap operator()(const LanguageInfo& language) const;

    private:
        std::string _filePath;
        char _delimeter;
    };
}