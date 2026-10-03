#include "LocalizationProvider.h"
#include "../Logging.h"
#include "../Exceptions.h"
#include "../Tools/CSVReader.h"

#include <fstream>

namespace Sgl
{
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
        LocalizationMap GetLocalization(std::string_view languageName) const
        {
            LocalizationMap localization;

            if(auto stream = std::ifstream(FilePath))
            {
                std::string line;
                if(std::getline(stream, line))
                {
                    auto headers = ParseLine(line, Delimiter);

                    int languageIndex = -1;
                    for(size_t i = 0; i < headers.size(); i++)
                    {
                        if(headers[i] == languageName)
                        {
                            languageIndex = static_cast<int>(i);
                            break;
                        }
                    }

                    if(languageIndex >= 0)
                    {
                        while(std::getline(stream, line))
                        {
                            auto record = ParseLine(line, Delimiter);
                            if(!record.empty() && languageIndex < static_cast<int>(record.size()))
                            {
                                localization.emplace(record[0], record[languageIndex]);
                            }
                        }
                    }
                }
            }
            else
            {
                Logging::LogWarning("Unable to open a csv file: '{}'.", FilePath);
            }

            return localization;
        }
    };

    InMemoryLocalizationProvider::InMemoryLocalizationProvider(std::unordered_map<LanguageInfo, Func<LocalizationMap>> factories):
        _factories(std::move(factories))
    {}

    LocalizationMap InMemoryLocalizationProvider::operator()(const LanguageInfo& language) const
    {
        auto it = _factories.find(language);
        return it != _factories.end() ? it->second() : LocalizationMap();
    }

    void InMemoryLocalizationProvider::AddLanguage(LanguageInfo languange, Func<LocalizationMap> factory)
    {
        _factories.emplace(std::move(languange), std::move(factory));
    }

    bool InMemoryLocalizationProvider::HasLanguage(const LanguageInfo& language) const
    {
        return _factories.find(language) != _factories.end();
    }

    CSVLocalizationProvider::CSVLocalizationProvider(std::string filePath, char delimeter):
        _filePath(std::move(filePath)),
        _delimeter(delimeter)
    {}

    LocalizationMap CSVLocalizationProvider::operator()(const LanguageInfo& language) const
    {
        LocalizationCSVReader csvReader(_filePath, _delimeter);
        return csvReader.GetLocalization(language.Name);
    }
}