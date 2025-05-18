#pragma once

#include <vector>

namespace Sgl
{		
	template<typename T>
	struct StyleProperties;

	template<typename TTargetType,
			 typename TProperties = StyleProperties<TTargetType>,
			 typename TStyleSelector = void(*)(TProperties&)>
	class Style
	{
	private:
		using StyleSelector = TStyleSelector;

		TProperties& _properties;
		std::vector<StyleSelector> _selectors;
		Style& _base = *this;
	public:
		Style(TProperties& properties):
			_properties(properties)
		{}

		Style(TProperties& properties, Style& base):
			_properties(properties),
			_base(base)
		{}

		Style(const Style&) = delete;
		Style(Style&&) = delete;

		void Use(std::vector<StyleSelector> selectors)
		{
			_selectors = std::move(selectors);
			ApplyStyle(_properties);
		}

		template<StyleSelector... Selectors>
		void Use()
		{
			Use({ Selectors... });
		}

		void Apply()
		{
			_properties = {};
			if(this != &_base)
			{
				_base.ApplyStyle(_properties);
			}

			ApplyStyle(_properties);
		}
	private:
		void ApplyStyle(TProperties& properties)
		{
			for(StyleSelector selector : _selectors)
			{				
				selector(properties);
			}
		}
	};
}