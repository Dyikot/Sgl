#pragma once

#include <string>
#include "../Event.h"

namespace Sgl
{
	/// <summary>
	/// Represents information about a language, including its name and provides functionality
	/// to manage the current application language.
	/// </summary>
	class LanguageInfo
	{
	private:
		using LanguageChangedEventHandler = EventHandler<LanguageInfo>;
	public:
		/// <summary>
		/// Constructs a LanguageInfo object with the specified language name.
		/// </summary>
		/// <param name="name"> - the name of the language.</param>
		explicit LanguageInfo(std::string name);		
		LanguageInfo(const LanguageInfo&) = default;
		LanguageInfo(LanguageInfo&&) noexcept = default;

		/// <summary>
		/// Event triggered when the current language changes.
		/// </summary>
		static inline Event<LanguageChangedEventHandler> CurrentChanged;

		/// <summary>
		/// Sets the current application language to the specified language info.
		/// </summary>
		/// <param name="languageInfo"> - the language info to set as current.</param>
		static void SetCurrent(const LanguageInfo& languageInfo);
		
		/// <summary>
		/// Gets the current application language.
		/// </summary>
		/// <returns>A const reference to the current language info.</returns>
		static const LanguageInfo& GetCurrent();

		/// <summary>
		/// Gets the name of the language.
		/// </summary>
		/// <returns>A const reference to the language name.</returns>
		const std::string& GetName() const noexcept { return _name; }

		LanguageInfo& operator=(const LanguageInfo&) = default;
		LanguageInfo& operator=(LanguageInfo&&) noexcept = default;

		/// <summary>
		/// Compares this LanguageInfo object with another for equality based on language name.
		/// </summary>
		/// <param name="other"> - the other LanguageInfo object to compare with.</param>
		/// <returns>True if both objects have the same language name, false otherwise.</returns>
		bool operator==(const LanguageInfo& other) const { return _name == other._name; }
	private:
		std::string _name;
	};
}