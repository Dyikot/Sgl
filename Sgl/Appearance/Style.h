#pragma once

#include <unordered_map>

namespace Sgl
{	
	enum class StyleState
	{
		Normal, Hover
	};
	
	template<typename TTarget>
	class Style
	{
	private:
		using StyleSelector = void(*)(TTarget&);
		static constexpr auto EmptySelector = [](TTarget& style) {};

		class Setter
		{
		private:
			Style& _style;
			StyleState _state;
		public:
			Setter(Style& style, StyleState state): 
				_style(style), _state(state)
			{}

			template<StyleSelector... Selectors>
			void Use()
			{
				auto& selector = _style._selectors[_state];
				selector = CombineSelectors<Selectors...>;
				selector(_style._target);
			}
		};

		std::unordered_map<StyleState, StyleSelector> _selectors;
		TTarget& _target;
	public:
		explicit Style(TTarget& target):
			_target(target)
		{
			AddState(StyleState::Normal);
		}

		void AddState(StyleState state)
		{
			_selectors[state] = EmptySelector;
		}

		Setter On(StyleState state)
		{
			return Setter(*this, state);
		}

		void ApplyStyleOn(StyleState state)
		{
			_target = {};
			SetStyleTo(StyleState::Normal);

			if(state != StyleState::Normal)
			{
				SetStyleTo(state);
			}
		}
	private:
		void SetStyleTo(StyleState state)
		{
			auto selector = _selectors[state];

			if(selector)
			{
				selector(_target);
			}
		}

		template<StyleSelector... Selectors>
		static void CombineSelectors(TTarget& target)
		{
			(Selectors(target), ...);
		}
	};
}