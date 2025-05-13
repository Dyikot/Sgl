#pragma once

#include <vector>

namespace Sgl
{		
	template<typename TTargetType,
			 typename TStyleSelector = void(*)(TTargetType&), 
			 typename TResetFactory = TTargetType(*)()>
	class Style
	{
	private:
		using StyleSelector = TStyleSelector;
		using ResetFactory = TResetFactory;
		static constexpr auto DefaultResetFactory = [] { return TTargetType(); };

		TTargetType& _target;
		std::vector<StyleSelector> _selectors;
		ResetFactory _resetFactory;
		Style& _base = *this;
	public:
		Style(TTargetType& target, ResetFactory resetFactory = DefaultResetFactory):
			_target(target),
			_resetFactory(resetFactory)
		{}

		Style(TTargetType& target,
			  Style& base,
			  ResetFactory resetFactory = DefaultResetFactory):
			_target(target),
			_resetFactory(resetFactory)
		{}

		Style(const Style&) = delete;
		Style(Style&&) = delete;

		void Use(std::vector<StyleSelector> selectors)
		{
			_selectors = std::move(selectors);
			ApplyStyle(_target);
		}

		template<StyleSelector... Selectors>
		void Use()
		{
			Use({ Selectors... });
		}

		void Apply()
		{
			_target = _resetFactory();
			if(this != &_base)
			{
				_base.ApplyStyle(_target);
			}

			ApplyStyle(_target);
		}
	private:
		void ApplyStyle(TTargetType& target)
		{
			for(StyleSelector selector : _selectors)
			{
				selector(target);
			}
		}
	};
}