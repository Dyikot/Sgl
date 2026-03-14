#pragma once

#include <bitset>
#include <string_view>
#include <unordered_map>
#include "../Base/Event.h"

namespace Sgl 
{
	using PseudoClassId = unsigned long long;

	class PseudoClassesRegistry
	{
	public:
		static PseudoClassId Register(std::string_view name);
		static PseudoClassId GetByName(std::string_view name);
	private:
		static inline PseudoClassId _nextId = 0;
		static inline std::unordered_map<std::string_view, PseudoClassId> _nameToId;
	};	

	class PseudoClassesSet
	{
	public:
		using ChangedEventHandler = EventHandler<PseudoClassesSet&>;
		using PseudoClasses = std::bitset<64>;
	public:
		PseudoClassesSet() = default;
		explicit PseudoClassesSet(PseudoClasses classes);
		PseudoClassesSet(const PseudoClassesSet& other);
		PseudoClassesSet(PseudoClassesSet&&) noexcept = default;

		Event<ChangedEventHandler> Changed;

		void Set(PseudoClassId pseudoClass, bool value = true);
		void Reset(PseudoClassId pseudoClass);
		bool IsEmpty() const noexcept;
		bool Has(PseudoClassId pseudoClass) const;
		bool Has(PseudoClasses pseudoClasses) const;
		bool Has(const PseudoClassesSet& pseudoClasses) const;
	private:
		PseudoClasses _classes;
	};
}