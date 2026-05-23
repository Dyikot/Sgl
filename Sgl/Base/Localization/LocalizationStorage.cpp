#include "LocalizationStorage.h"
#include "../Logging.h"
#include "../Exceptions.h"

#include <fstream>

namespace Sgl
{
	LocalizationStorage::LocalizationStorage(LanguageManager& languageManager, 
											 LocalizationLoader localizationLoader):
		_languageManager(languageManager),
		_loader(std::move(localizationLoader))
	{
		LoadLocalizationStrings(languageManager.GetCurrent());
		_languageManager.CurrentChanged += MethodEventHandler(&LocalizationStorage::OnLanguageChanged, this);
	}

	LocalizationStorage::~LocalizationStorage()
    {
        _languageManager.CurrentChanged -= MethodEventHandler(&LocalizationStorage::OnLanguageChanged, this);
    }

    std::string LocalizationStorage::GetLocalizedString(std::string_view key) const
    {
        if(auto it = _map.find(key); it != _map.end())
        {
            return it->second;
        }

        return std::string(key);
    }

    void LocalizationStorage::OnLanguageChanged(LanguageManager& sender, EventArgs e)
    {
        LoadLocalizationStrings(sender.GetCurrent());
    }

    void LocalizationStorage::LoadLocalizationStrings(const LanguageInfo& languageInfo)
    {
        _map = _loader(languageInfo);

        if(_map.empty())
        {
            Logging::LogWarning("Failed to load a localization for '{}' language.", languageInfo.Name);
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