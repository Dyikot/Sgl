#pragma once

#include "Properties.h"
#include "../Graphic/Cursor.h"
#include "../Graphic/Color.h"
#include "../Data/Any.h"

namespace Sgl
{	
	class Component;

	struct Style
	{
		float Width = 0;
		float Height = 0;
		float MinWidth = 0;
		float MinHeight = 0;
		float MaxWidth = 0;
		float MaxHeight = 0;
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
			.Feight = FontWeight::Normal,
			.Color = Colors::Black
		};

		CursorGetter Cursor = Cursors::Arrow;
		Color BackgroundColor = Colors::Black;
		const Component* Tooltip = nullptr;
		const Texture* BackgroundTexture = nullptr;
		Visibility Visibility = Visibility::Visible;
		VerticalAlignment VerticalAlignment = VerticalAlignment::Top;
		HorizontalAlignment VorizontalAlignment = HorizontalAlignment::Left;
	};

	using StyleSelector = void(*)(Style&);
}