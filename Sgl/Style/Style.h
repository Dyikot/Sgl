#pragma once

#include <optional>
#include <type_traits>
#include <string>
#include "../Data/Delegate.h"

namespace Sgl
{
	class StyleableElement;
	using Setter = void(*)(StyleableElement&);

	enum class Trigger
	{
		Hover, Pressed
	};

	class Selector
	{
	private:
		std::string _name;
		std::string _class;
		std::optional<Trigger> _trigger;
		std::reference_wrapper<const type_info> _type;
	public:
		Selector():
			_type(typeid(nullptr))
		{}

		template<std::derived_from<StyleableElement> T>
		Selector& OfType()
		{
			_type = typeid(T);
			return *this;
		}

		Selector& Name(std::string value)
		{ 
			_name = std::move(value);
			return *this;
		}

		Selector& Class(std::string value)
		{
			_class = std::move(value);
			return *this;
		}

		Selector& On(Trigger value)
		{
			_trigger = value;
			return *this;
		}
	};

	struct Style
	{
		Selector Selector;
		Setter Setter;
	};
}