#pragma once

#include <bitset>
#include <string_view>
#include <unordered_map>
#include "../Base/Event.h"

namespace Sgl
{
	/// <summary>
	/// Unique identifier for a pseudo-class.
	/// </summary>
	using PseudoClassId = unsigned long long;

	/// <summary>
	/// Registry for pseudo-class names and their unique identifiers.
	/// Provides global registration and lookup of pseudo-classes by name.
	/// </summary>
	class PseudoClassesRegistry
	{
	public:
		/// <summary>
		/// Registers a new pseudo-class with the specified name.
		/// </summary>
		/// <param name="name"> - the pseudo-class name.</param>
		/// <returns>The unique identifier for the registered pseudo-class.</returns>
		static PseudoClassId Register(std::string_view name);

		/// <summary>
		/// Gets the identifier for a pseudo-class by name.
		/// </summary>
		/// <param name="name"> - the pseudo-class name.</param>
		/// <returns>The unique identifier</returns>
		static PseudoClassId GetByName(std::string_view name);
	private:
		static inline PseudoClassId _nextId = 0;
		static inline std::unordered_map<std::string_view, PseudoClassId> _nameToId;
	};

	/// <summary>
	/// Represents a set of active pseudo-classes.
	/// Supports change notification when the set is modified.
	/// </summary>
	class PseudoClassesSet
	{
	public:
		/// <summary>
		/// Event handler type for pseudo-class set changes.
		/// </summary>
		using ChangedEventHandler = EventHandler<PseudoClassesSet&>;
		using PseudoClasses = std::bitset<64>;
	public:
		PseudoClassesSet() = default;
		explicit PseudoClassesSet(PseudoClasses classes);
		PseudoClassesSet(const PseudoClassesSet& other);
		PseudoClassesSet(PseudoClassesSet&&) noexcept = default;

		/// <summary>
		/// Event raised when the pseudo-class set changes.
		/// </summary>
		Event<ChangedEventHandler> Changed;

		/// <summary>
		/// Sets or resets a pseudo-class in the set.
		/// </summary>
		/// <param name="pseudoClass"> - the pseudo-class identifier.</param>
		/// <param name="value"> - true to set, false to reset.</param>
		void Set(PseudoClassId pseudoClass, bool value = true);

		/// <summary>
		/// Resets a pseudo-class in the set.
		/// </summary>
		/// <param name="pseudoClass"> - the pseudo-class identifier.</param>
		void Reset(PseudoClassId pseudoClass);

		/// <summary>
		/// Determines whether the set is empty.
		/// </summary>
		/// <returns>True if no pseudo-classes are set, false otherwise.</returns>
		bool IsEmpty() const noexcept;

		/// <summary>
		/// Checks if a specific pseudo-class is set.
		/// </summary>
		/// <param name="pseudoClass"> - the pseudo-class identifier.</param>
		/// <returns>True if the pseudo-class is set, false otherwise.</returns>
		bool Has(PseudoClassId pseudoClass) const;

		/// <summary>
		/// Checks if all pseudo-classes in a mask are set.
		/// </summary>
		/// <param name="pseudoClasses"> - the pseudo-class mask.</param>
		/// <returns>True if all pseudo-classes in the mask are set, false otherwise.</returns>
		bool Has(PseudoClasses pseudoClasses) const;

		/// <summary>
		/// Checks if all pseudo-classes in another set are set.
		/// </summary>
		/// <param name="pseudoClasses"> - the pseudo-class set to check.</param>
		/// <returns>True if all pseudo-classes are set, false otherwise.</returns>
		bool Has(const PseudoClassesSet& pseudoClasses) const;
	private:
		PseudoClasses _classes;
	};
}