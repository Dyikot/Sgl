#pragma once

#include "../Data/Object.h"
#include <unordered_map>
#include <string_view>
#include "../Graphic/Cursor.h"
#include "Properties.h"
#include "../Graphic/Color.h"

namespace Sgl
{
	class Component;

	class Style
	{
	public:
		std::reference_wrapper<const Cursor> cursor = Cursors::Arrow();
		float width = 0;
		float height = 0;
		float minWidth = 0;
		float minHeight = 0;
		float maxWidth = 0;
		float maxHeight = 0;
		size_t zIndex = 1;
		Thickness margin;
		Nullable<const Component> tooltip;
		HorizontalAlignment horizontalAlignment = HorizontalAlignment::Left;
		VerticalAlignment verticalAlignment = VerticalAlignment::Top;
		Visibility visibility = Visibility::Visible;
		Color borderColor = Colors::Black;
		FontFamily fontFamily;
		FontWeight fontWeight = FontWeight::Normal;
		Thickness borderThickness;
		int fontSize = 14;
		Color fontColor = Colors::Black;
	protected:
		std::unordered_map<std::string_view, object> _properties;
	public:
		Style();

		object GetByName(std::string_view name);
	};
}