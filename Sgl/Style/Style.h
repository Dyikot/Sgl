#pragma once

#include "../Data/Object.h"
#include <unordered_map>
#include <string_view>
#include "../Graphic/Cursor.h"
#include "Properties.h"
#include "../Graphic/Color.h"
#include "../Data/CallableView.h"
#include "../Data/Any.h"

namespace Sgl
{	
	class Component;

	struct Style
	{
		float width = 0;
		float height = 0;
		float minWidth = 0;
		float minHeight = 0;
		float maxWidth = 0;
		float maxHeight = 0;
		size_t zIndex = 1;
		Thickness margin;
		Thickness padding;
		Border border =
		{
			.thickness = Thickness(1),
			.color = Colors::Black
		};
		Font font =
		{
			.size = 14,
			.family = {},
			.style = FontStyle::Normal,
			.weight = FontWeight::Normal,
			.color = Colors::Black
		};
		Nullable<const Component> tooltip;
		Nullable<Texture> backgroundTexture;
		Color backgroundColor = Colors::Black;
		Visibility visibility = Visibility::Visible;
		std::reference_wrapper<const Cursor> cursor = Cursors::Arrow();
		VerticalAlignment verticalAlignment = VerticalAlignment::Top;
		HorizontalAlignment horizontalAlignment = HorizontalAlignment::Left;
	};
}