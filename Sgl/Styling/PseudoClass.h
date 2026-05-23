#pragma once

#include <bitset>
#include <string_view>
#include <unordered_map>
#include "../Base/Event.h"

namespace Sgl
{
	/// <summary>
	/// Represents a class that denotes a specific state. For example, "hover," "pressed," etc.
	/// </summary>
	class PseudoClass
	{
	public:
		/// <summary>
		/// Register a PseudoClass from a name. 
		/// A new unique ID is assigned (sequentially from 0 up to 63).
		/// </summary>
		/// <param name="name">- the string identifier for the pseudo-class.</param>
		explicit PseudoClass(std::string_view name);
		PseudoClass(const PseudoClass&) = default;
		PseudoClass(PseudoClass&&) noexcept = default;

		/// <summary>
		/// Retrieves an existing PseudoClass by name without modifying the registry.
		/// </summary>
		/// <param name="name"> - the string identifier to look up.</param>
		/// <returns></returns>
		static PseudoClass GetByName(std::string_view name);

		/// <summary>
		/// Returns the unique numeric identifier for this instance.
		/// </summary>
		/// <returns>The assigned ID, guaranteed to be in the range [0, 63] for valid instances.</returns>
		size_t GetId() const noexcept { return _id; }
	private:
		explicit PseudoClass(size_t id);
	private:
		static inline std::unordered_map<std::string_view, size_t> _registry;
		size_t _id = 0;
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
		/// <param name="pseudoClass"> - the pseudo-class.</param>
		/// <param name="value"> - true to set, false to reset.</param>
		void Set(PseudoClass pseudoClass, bool value = true);

		/// <summary>
		/// Resets a pseudo-class in the set.
		/// </summary>
		/// <param name="pseudoClass"> - the pseudo-class.</param>
		void Reset(PseudoClass pseudoClass);

		/// <summary>
		/// Determines whether the set is empty.
		/// </summary>
		/// <returns>True if no pseudo-classes are set, false otherwise.</returns>
		bool IsEmpty() const noexcept;

		/// <summary>
		/// Checks if a specific pseudo-class is set.
		/// </summary>
		/// <param name="pseudoClass"> - the pseudo-class.</param>
		/// <returns>True if the pseudo-class is set, false otherwise.</returns>
		bool Has(PseudoClass pseudoClass) const;

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