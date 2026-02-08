#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include "LanguageInfo.h"

namespace Sgl
{
    /// <summary>
    /// Manages localization data loaded from a CSV file, providing access to localized strings
    /// based on keys and current language settings.
    /// </summary>
    class LocalizationStorage
    {
    public:
        /// <summary>
        /// Constructs a LocalizationStorage object by loading data from the specified CSV file.
        /// </summary>
        /// <param name="csvFilePath"> - path to the CSV file containing localization data.</param>
        /// <param name="delimeter"> - character used as delimiter in the CSV file (default is comma).</param>
        LocalizationStorage(std::string csvFilePath, char delimeter = ',');
        LocalizationStorage(const LocalizationStorage&) = delete;
        LocalizationStorage(LocalizationStorage&&) = default;
        ~LocalizationStorage();

        /// <summary>
        /// Retrieves the localized string for the specified key.
        /// </summary>
        /// <param name="key"> - the localization key to look up.</param>
        /// <returns>A const reference to the localized string.</returns>
        const std::string& GetLocalizedString(const std::string& key) const;
    private:
        void OnLanguageChanged(LanguageInfo& sender, EventArgs e);
        void CreateKeyIndex();
        void SetLanguage(const LanguageInfo& language);
    private:
        std::vector<std::string> _headers;
        std::vector<std::string> _records;
        uint32_t _headersCount = 0;
        int _languageIndex = 0;
        std::unordered_map<std::string, uint32_t> _keyIndex;
    };
}