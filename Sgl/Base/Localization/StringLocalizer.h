#pragma once

#include <format>

namespace Sgl
{
	/// <summary>
	/// A convenient callable wrapper for localized string retrieval with optional formatting.
	/// Delegates key lookup to the global localization storage and supports std::format-style arguments.
	/// </summary>
	class StringLocalizer
	{
	public:
		/// <summary>
		/// Retrieves the localized string for the given key without formatting.
		/// </summary>
		/// <param name="key"> - the localization key.</param>
		/// <returns>A const reference to the localized string.</returns>
		const std::string& operator()(const std::string& key) const;

		/// <summary>
		/// Retrieves the localized string for the given key and formats it using the provided arguments.
		/// Uses std::vformat internally, so the localized string must contain valid format specifiers.
		/// </summary>
		/// <param name="key"> - the localization key.</param>
		/// <param name="args"> - arguments to substitute into the formatted string.</param>
		/// <returns>A formatted std::string.</returns>
		template<typename... TArgs>
		std::string operator()(const std::string& key, TArgs&&... args) const
		{
			return std::vformat(operator()(key), std::make_format_args(args...));
		}
	};

	/// <summary>
	/// Global instance of StringLocalizer for easy access to localized strings anywhere in the application.
	/// Usage examples:
	///   auto message = Localizer("WelcomeMessage");
	///   auto formatted = Localizer("HelloUser", userName);
	/// </summary>
	inline StringLocalizer Localizer;
}