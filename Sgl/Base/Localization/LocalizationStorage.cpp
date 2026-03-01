#include "LocalizationStorage.h"
#include "../Logging.h"
#include "../Exceptions.h"

namespace Sgl
{
    LocalizationStorage::LocalizationStorage(LocalizationLoader localizationLoader):
        _localizationLoader(std::move(localizationLoader))
    {
        LoadLocalizationStrings(LanguageInfo::GetCurrent());
        LanguageInfo::CurrentChanged += MethodEventHandler(&LocalizationStorage::OnLanguageChanged, this);
    }

    LocalizationStorage::~LocalizationStorage()
    {
        LanguageInfo::CurrentChanged -= MethodEventHandler(&LocalizationStorage::OnLanguageChanged, this);
    }

    std::string LocalizationStorage::GetLocalizedString(const std::string& key) const
    {
        if(auto it = _localizedStrings.find(key); it != _localizedStrings.end())
        {
            return it->second;
        }

        return key;
    }

    void LocalizationStorage::OnLanguageChanged(LanguageInfo& sender, EventArgs e)
    {
        LoadLocalizationStrings(sender);
    }

    void LocalizationStorage::LoadLocalizationStrings(const LanguageInfo& languageInfo)
    {
        _localizedStrings = _localizationLoader(languageInfo);

        if(_localizedStrings.empty())
        {
            Logging::LogWarning("Failed to get localization for '{}' language.", languageInfo.GetName());
        }
    }
}