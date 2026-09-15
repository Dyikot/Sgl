#pragma once

#include <bitset>
#include <unordered_map>

#include "../Base/Event.h"
#include "../Base/Tools/StringUtils.h"

namespace Sgl
{
	//! @brief Represents a class that denotes a specific state, such as "hover" or "pressed"
	class PseudoClass
	{
	public:
		//! @brief Registers a PseudoClass from a name. A new unique ID is assigned sequentially from 0 up to 63.
		//! @param name The string identifier for the pseudo-class
		//! @return PseudoClass instance
		static PseudoClass Register(std::string_view name);

		//! @brief Retrieves an existing PseudoClass by name without modifying the registry
		//! @param name The string identifier to look up
		//! @return PseudoClass instance
		static PseudoClass GetByName(std::string_view name);

		//! @brief Returns the unique numeric identifier for this instance
		//! @return The assigned ID, guaranteed to be in the range [0, 63] for valid instances
		size_t GetId() const noexcept { return _id; }

	private:
		explicit PseudoClass(size_t id);

	private:
		static inline std::unordered_map<std::string, size_t, StringHash, std::equal_to<>> _registry;
		size_t _id = 0;
	};

	//! @brief Represents a set of active pseudo-classes that supports change notification when modified
	class PseudoClassesSet
	{
	public:
		//! @brief Event handler type for pseudo-class set changes
		using ChangedEventHandler = EventHandler<PseudoClassesSet&>;
		using PseudoClasses = std::bitset<64>;

	public:
		//! @brief Default constructor
		PseudoClassesSet() = default;

		//! @brief Constructs a set from a bitmask of pseudo-classes
		//! @param classes The bitmask of pseudo-classes
		explicit PseudoClassesSet(PseudoClasses classes);

		PseudoClassesSet(const PseudoClassesSet&) = default;
		PseudoClassesSet(PseudoClassesSet&&) noexcept = default;

		//! @brief Event raised when the pseudo-class set changes
		Event<ChangedEventHandler> Changed;

		//! @brief Sets or resets a pseudo-class in the set
		//! @param pseudoClass The pseudo-class
		//! @param value True to set, false to reset
		void Set(PseudoClass pseudoClass, bool value = true);

		//! @brief Resets a pseudo-class in the set
		//! @param pseudoClass The pseudo-class
		void Reset(PseudoClass pseudoClass);

		//! @brief Determines whether the set is empty
		//! @return True if no pseudo-classes are set; otherwise, false
		bool IsEmpty() const noexcept;

		//! @brief Checks if a specific pseudo-class is set
		//! @param pseudoClass The pseudo-class
		//! @return True if the pseudo-class is set; otherwise, false
		bool Has(PseudoClass pseudoClass) const;

		//! @brief Checks if all pseudo-classes in a mask are set
		//! @param pseudoClasses The pseudo-class mask
		//! @return True if all pseudo-classes in the mask are set; otherwise, false
		bool Has(PseudoClasses pseudoClasses) const;

		//! @brief Checks if all pseudo-classes in another set are set
		//! @param pseudoClasses The pseudo-class set to check
		//! @return True if all pseudo-classes are set; otherwise, false
		bool Has(const PseudoClassesSet& pseudoClasses) const;

	private:
		PseudoClasses _classes;
	};
}