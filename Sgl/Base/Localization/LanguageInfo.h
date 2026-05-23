#pragma once

#include "../Event.h"
#include <string>

namespace Sgl
{
	/// <summary>
	/// Represents basic information about a language.
	/// </summary>
	struct LanguageInfo
	{
		/// <summary>
		/// The identifier of the language (e.g., "en-US", "fr-FR", "de-DE").
		/// </summary>
		std::string Name;
	};

	/// <summary>
	/// Manages the application's current language state and notifies subscribers when it changes.
	/// </summary>
	class LanguageManager
	{
	public:
		using CurrentChangedEventHandler = EventHandler<LanguageManager>;

		Event<CurrentChangedEventHandler> CurrentChanged;

		void SetCurrent(const LanguageInfo& languageInfo);
		const LanguageInfo& GetCurrent() const;
	private:
		LanguageInfo _current { "en-US" };
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