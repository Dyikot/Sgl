#include "Style.h"

namespace Sgl
{
	Style::Style()
	{ 
        _properties["Cursor"] = cursor;
        _properties["Width"] = width;
        _properties["Height"] = height;
        _properties["MinWidth"] = minWidth;
        _properties["MinHeight"] = minHeight;
        _properties["MaxWidth"] = maxWidth;
        _properties["MaxHeight"] = maxHeight;
        _properties["ZIndex"] = zIndex;
        _properties["Margin"] = margin;
        _properties["Tooltip"] = tooltip;
        _properties["HorizontalAlignment"] = horizontalAlignment;
        _properties["VerticalAligment"] = verticalAlignment;
        _properties["Visibility"] = visibility;
        _properties["BorderColor"] = borderColor;
        _properties["FontFamily"] = fontFamily;
        _properties["FontWeight"] = fontWeight;
        _properties["BorderThickness"] = borderThickness;
        _properties["FontSize"] = fontSize;
        _properties["FontColor"] = fontColor;
	}

    object Style::GetByName(std::string_view name)
    {
        if(auto it = _properties.find(name); it != _properties.end())
        {
            return it->second;
        }

        return nullptr;
    }
}
