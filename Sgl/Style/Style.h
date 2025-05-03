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
		Thickness margin;
		size_t zIndex = 1;
		int fontSize = 14;
		FontFamily fontFamily;
		Thickness borderThickness;
		Color fontColor = Colors::Black;
		Nullable<const Component> tooltip;
		Color borderColor = Colors::Black;
		Nullable<Texture> backgroundTexture;
		Color backgroundColor = Colors::Black;
		FontWeight fontWeight = FontWeight::Normal;
		Visibility visibility = Visibility::Visible;
		std::reference_wrapper<const Cursor> cursor = Cursors::Arrow();
		VerticalAlignment verticalAlignment = VerticalAlignment::Top;
		HorizontalAlignment horizontalAlignment = HorizontalAlignment::Left;
	};
}