#pragma once

#include <string>

namespace Sgl
{
	//! @brief Represents basic information about a language
	struct LanguageInfo
	{
		//! @brief The identifier of the language (e.g., "en", "fr", "de")
		std::string Name;

		bool operator==(const LanguageInfo&) const = default;
	};
}

namespace std
{
	template<>
	struct hash<Sgl::LanguageInfo>
	{
		size_t operator()(const Sgl::LanguageInfo& info) const noexcept
		{
			return hash<string>()(info.Name);
		}
	};
}