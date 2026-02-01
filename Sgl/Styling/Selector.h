#pragma once

#include <string>

namespace Sgl
{
	class StyleableElement;

	class Selector
	{
	private:
		using TypeComparer = bool(*)(StyleableElement&);
	public:
		Selector() = default;
		Selector(const Selector&) = default;
		Selector(Selector&&) = default;

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

		Selector& Class(std::string className)
		{
			_className = std::move(className);
			_flags |= ClassFlag;
			return *this;
		}

		bool Match(StyleableElement& target) const;
	private:
		template<typename T>
		static bool CompareType(StyleableElement& target)
		{
			return dynamic_cast<T*>(&target);
		}

		template<typename T>
		static bool CompareTypeById(StyleableElement& target)
		{
			return typeid(T) == typeid(target);
		}
	private:
		static constexpr uint32_t TypeFlag = 0x1;
		static constexpr uint32_t ClassFlag = 0x2;

		std::string _className;
		TypeComparer _typeComparer = nullptr;
		uint32_t _flags = 0;
	};
}