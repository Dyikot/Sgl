#pragma once

#include <format>

namespace Sgl
{
	//! @brief A convenient callable wrapper for localized string retrieval with optional formatting
	class StringLocalizer
	{
	public:
		//! @brief Retrieves the localized string for the given key without formatting
		//! @param key The localization key
		//! @return The localized string
		std::string operator()(std::string_view key) const;
				
		//! @brief Retrieves the localized string for the given key and formats it using the provided arguments
		//! @param key The localization key
		//! @param ...args Arguments to substitute into the formatted string
		//! @return A formatted std::string
		template<typename... TArgs>
		std::string operator()(std::string_view key, TArgs&&... args) const
		{
			return std::vformat(operator()(key), std::make_format_args(args...));
		}
	};

	//! @brief Global instance of StringLocalizer for easy access to localized strings anywhere in the application
	inline constexpr StringLocalizer Localizer;
}