#pragma once

#include <string>
#include <memory>

namespace Sgl::Localization
{
    /// <summary>
    /// Abstract interface for a localization storage backend that provides culture-specific string resources.
    /// Supports switching cultures and retrieving localized strings by key.
    /// </summary>
    class ILocalizationStorage
    {
    public:
        virtual ~ILocalizationStorage() = default;

        /// <summary>
        /// Sets the current culture (e.g., "en-US", "fr-FR") used for localization lookups.
        /// </summary>
        /// <param name="culture"> - the culture identifier string.</param>
        virtual void SetCulture(const std::string& culture) = 0;

        /// <summary>
        /// Returns the currently active culture identifier.
        /// </summary>
        /// <returns>A reference to the current culture string.</returns>
        virtual const std::string& GetCulture() const = 0;

        /// <summary>
        /// Retrieves the localized string associated with the given key in the current culture.
        /// If the key is not found, implementations return the key itself.
        /// </summary>
        /// <param name="key"> - the localization key.</param>
        /// <returns>A reference to the localized string.</returns>
        virtual const std::string& GetLocalizedString(const std::string& key) const = 0;
    };

    /// <summary>
    /// Registers a custom localization storage implementation as the global localization provider.
    /// </summary>
    /// <param name="storage"> - a unique pointer to an ILocalizationStorage implementation.</param>
    void SetLocalizationStorage(std::unique_ptr<ILocalizationStorage> storage);

    /// <summary>
    /// Initializes the global localization system using a CSV file as the backing store.
    /// The CSV file must contain at least two columns: the first is the key, and subsequent columns
    /// correspond to localized values for each supported culture (header row defines culture names).
    /// </summary>
    /// <param name="csvFilePath">Path to the CSV localization file.</param>
    /// <param name="delimeter">Field delimiter character (default: ',').</param>
    void SetLocalizationStorage(std::string csvFilePath, char delimeter = ',');

    /// <summary>
    /// Returns a reference to the currently active global localization storage.
    /// </summary>
    /// <returns>A const reference to the unique_ptr managing the current ILocalizationStorage instance.</returns>
    const std::unique_ptr<ILocalizationStorage>& GetLocalizationStorage();
}