#pragma once

#include <vector>

namespace Sgl
{		
	template<typename TTargetType,
			 typename TTargetProperty = std::unique_ptr<TTargetType>,
			 typename TStyleSelector = void(*)(TTargetType&),
			 typename TResetFactory = TTargetType(*)()>
	class Style
	{
	private:
		using StyleSelector = TStyleSelector;
		using ResetFactory = TResetFactory;
		static constexpr auto DefaultResetFactory = [] { return TTargetType(); };

		TTargetProperty& _target;
		std::vector<StyleSelector> _selectors;
		ResetFactory _resetFactory;
		Style& _base = *this;
	public:
		Style(TTargetProperty& target, ResetFactory resetFactory = DefaultResetFactory) :
			_target(target),
			_resetFactory(resetFactory)
		{}

		Style(TTargetProperty& target,
			  Style& base,
			  ResetFactory resetFactory = DefaultResetFactory) :
			_target(target),
			_base(base),
			_resetFactory(resetFactory)
		{}

		Style(const Style&) = delete;
		Style(Style&&) = delete;

		void Use(std::vector<StyleSelector> selectors)
		{
			_selectors = std::move(selectors);
			ApplyStyle(*_target);
		}

		template<StyleSelector... Selectors>
		void Use()
		{
			Use({ Selectors... });
		}

		void Apply()
		{
			auto& target = *_target;

			target = _resetFactory();
			if(this != &_base)
			{
				_base.ApplyStyle(target);
			}

			ApplyStyle(target);
		}
	private:
		void ApplyStyle(TTargetType& properties)
		{
			for(StyleSelector selector : _selectors)
			{				
				selector(properties);
			}
		}
	};
}