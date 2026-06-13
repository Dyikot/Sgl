#include "LocalizationResources.h"
#include "../Logging.h"
#include "../Exceptions.h"

#include <fstream>

namespace Sgl
{
	void LocalizationResources::SetProvider(LocalizationProvider localizationProvider)
	{
		_provider = std::move(localizationProvider);
		LoadLocalization();
	}

	void LocalizationResources::SetCSVProvider(std::string csvFilePath, char delimiter)
	{
		auto csvProvider = [csv = LocalizationCSVReader(std::move(csvFilePath), delimiter)]
			(const LanguageInfo& languageInfo)
		{
			return csv.GetLocalization(languageInfo.Name);
		};

		SetProvider(std::move(csvProvider));
	}

	void LocalizationResources::SetLanguage(const LanguageInfo& language)
	{
		if(_language == language)
		{
			return;
		}

		_language = language;
		LoadLocalization();
		LanguageChanged.Invoke(*this, _language);
	}

	std::string LocalizationResources::GetLocalizedString(std::string_view key) const
    {
        if(auto it = _map.find(key); it != _map.end())
        {
            return it->second;
        }

        return std::string(key);
    }

	void LocalizationResources::LoadLocalization()
	{
		if(_provider)
		{
			_map = _provider(_language);
		}
	}

	LocalizationMap LocalizationCSVReader::GetLocalization(std::string_view languageName) const
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
}