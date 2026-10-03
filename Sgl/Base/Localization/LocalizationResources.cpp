#include "LocalizationResources.h"

namespace Sgl
{
	void LocalizationResources::SetProvider(LocalizationProvider localizationProvider)
	{
		_provider = std::move(localizationProvider);
		LoadLocalization();
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
}