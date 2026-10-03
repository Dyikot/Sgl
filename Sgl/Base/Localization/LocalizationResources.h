#pragma once

#include "LocalizationProvider.h"
#include "../Event.h"

namespace Sgl
{ 
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
}