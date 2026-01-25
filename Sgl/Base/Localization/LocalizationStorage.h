#pragma once

#include <string>
#include <memory>

namespace Sgl::Localization
{
    class ILocalizationStorage
    {
    public:
        virtual ~ILocalizationStorage() = default;

        virtual void SetCulture(const std::string& culture) = 0;
        virtual const std::string& GetCulture() const = 0;

        virtual const std::string& GetLocalizedString(const std::string& key) const = 0;        
    };

    void SetLocalizationStorage(std::unique_ptr<ILocalizationStorage> storage);
    void SetLocalizationStorage(std::string csvFilePath, char delimeter = ',');
    const std::unique_ptr<ILocalizationStorage>& GetLocalizationStorage();
}