#pragma once

#include <string>

namespace Sgl
{
	/// <summary>
	/// Represents basic information about a language.
	/// </summary>
	struct LanguageInfo
	{
		/// <summary>
		/// The identifier of the language (e.g., "en", "fr", "de").
		/// </summary>
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