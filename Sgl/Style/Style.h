#pragma once

#include "Properties.h"
#include "../Graphic/Cursor.h"
#include "../Graphic/Color.h"
#include "../Data/Any.h"
#include "../Graphic/Texture.h"
#include <limits>

namespace Sgl
{	
	class Component;

	struct Style
	{
		float Width = 0;
		float Height = 0;
		float MinWidth = 0;
		float MinHeight = 0;
		float MaxWidth = std::numeric_limits<float>::max();
		float MaxHeight = std::numeric_limits<float>::max();
		size_t ZIndex = 1;
		Thickness Margin;
		Thickness Padding;

		Border Border =
		{
			.Thickness = Thickness(1),
			.Color = Colors::Black
		};

		Font Font =
		{
			.Size = 14,
			.Family = {},
			.Style = FontStyle::Normal,
			.Weight = FontWeight::Normal,
			.Color = Colors::Black
		};

		CursorGetter Cursor = Cursors::Arrow;
		Color BackgroundColor = Colors::Black;
		const Component* Tooltip = nullptr;
		const Texture* BackgroundTexture = nullptr;
		Visibility Visibility = Visibility::Visible;
		VerticalAlignment VerticalAlignment = VerticalAlignment::Top;
		HorizontalAlignment HorizontalAlignment = HorizontalAlignment::Left;
	};

	using StyleSelector = void(*)(Style&);

	enum class StyleTarget
	{
		Element, Hover
	};

	class StyleClass
	{
	private:
		static constexpr auto EmptySelector = [](Sgl::Style& style) {};

		std::unordered_map<StyleTarget, StyleSelector> _selectors;
		Style& _style;
	public:
		explicit StyleClass(Style& style):
			_style(style)
		{}

		template<StyleSelector... Selectors>
		void SetTo(StyleTarget styleTarget)
		{
			auto& selector = _selectors[styleTarget];
			selector = CombineSelectors<Selectors...>;
			selector(_style);
		}

		void AddTarget(StyleTarget styleTarget);
		void ApplyStyleTo(StyleTarget styleTarget);
	private:
		void SetStyleTo(StyleTarget styleTarget);

		template<StyleSelector... Selectors>
		static void CombineSelectors(Sgl::Style& style)
		{
			(Selectors(style), ...);
		}
	};
}