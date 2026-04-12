#pragma once

#include <string>
#include <vector>
#include "PseudoClass.h"
#include "../Base/Delegate.h"

namespace Sgl
{
	class StyleableElement;

	class Selector
	{
	private:
		using TypeComparer = bool(*)(const StyleableElement&);
		using PseudoClasses = std::bitset<64>;
	public:
		Selector() = default;
		Selector(const Selector&) = default;
		Selector(Selector&&) noexcept = default;

		template<typename T>
		Selector& OfType()
		{
			_typeComparer = CompareTypeById<T>;
			_flags |= TypeFlag;
			return *this;
		}

		template<typename T>
		Selector& Is()
		{
			_typeComparer = CompareType<T>;
			_flags |= TypeFlag;
			return *this;
		}

		Selector& Name(std::string name);
		Selector& Class(std::string className);
		Selector& On(PseudoClassId pseudoClass);
		Selector& On(std::string_view pseudoClass);
		Selector& Where(Predicate<const StyleableElement&> predicate);

		bool Match(const StyleableElement& target) const;
		bool MatchState(const StyleableElement& target) const;
		bool HasState() const noexcept;
	private:
		template<typename T>
		static bool CompareType(const StyleableElement& target)
		{
			return dynamic_cast<const T*>(&target);
		}

		template<typename T>
		static bool CompareTypeById(const StyleableElement& target)
		{
			return typeid(T) == typeid(target);
		}
	private:
		static constexpr uint32_t TypeFlag           = 1 << 0;
		static constexpr uint32_t NameFlag			 = 1 << 1;
		static constexpr uint32_t ClassFlag	         = 1 << 2;
		static constexpr uint32_t PredicateFlag		 = 1 << 3;

		uint32_t _flags = 0;
		TypeComparer _typeComparer = nullptr;
		std::string _name;
		std::vector<std::string> _classes;
		PseudoClasses _pseudoClasses;
		Predicate<const StyleableElement&> _predicate;
	};
}