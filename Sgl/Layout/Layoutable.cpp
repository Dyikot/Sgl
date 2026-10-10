#include "Layoutable.h"
#include "LayoutHelper.h"
#include "../Base/Logging.h"

#include <algorithm>

namespace Sgl
{
	void Layoutable::SetWidth(float value, ValueSource source)
	{
		SetProperty(WidthProperty, _width, value, _widthSource, source);
	}

	void Layoutable::SetHeight(float value, ValueSource source)
	{
		SetProperty(HeightProperty, _height, value, _heightSource, source);
	}

	void Layoutable::SetMinWidth(float value, ValueSource source)
	{
		SetProperty(MinWidthProperty, _minWidth, value, _minWidthSource, source);
	}

	void Layoutable::SetMinHeight(float value, ValueSource source)
	{
		SetProperty(MinHeightProperty, _minHeight, value, _minHeightSource, source);
	}

	void Layoutable::SetMaxWidth(float value, ValueSource source)
	{
		SetProperty(MaxWidthProperty, _maxWidth, value, _maxWidthSource, source);
	}

	void Layoutable::SetMaxHeight(float value, ValueSource source)
	{
		SetProperty(MaxHeightProperty, _maxHeight, value, _maxHeightSource, source);
	}

	void Layoutable::SetMargin(Thickness value, ValueSource source)
	{
		SetProperty(MarginProperty, _margin, value, _marginSource, source);
	}

	void Layoutable::SetIsVisible(bool value, ValueSource source)
	{
		SetProperty(IsVisibleProperty, _isVisible, value, _isVisibleSource, source);
	}

	void Layoutable::SetVerticalAlignment(VerticalAlignment value, ValueSource source)
	{
		SetProperty(VerticalAlignmentProperty, _verticalAlignment, value, _verticalAlignmentSource, source);
	}

	void Layoutable::SetHorizontalAlignment(HorizontalAlignment value, ValueSource source)
	{
		SetProperty(HorizontalAlignmentProperty, _horizontalAlignment, value, _horizontalAlignmentSource, source);
	}

	void Layoutable::Arrange(FRect rect)
	{
		if(!_isMeasureValid)
		{
			Measure(FSize(rect.w, rect.h));
		}

		if(IsVisible())
		{
			ArrangeCore(rect);
		}

		_isArrangeValid = true;
	}

	void Layoutable::Measure(FSize availableSize)
	{
		_desiredSize = IsVisible() ? MeasureCore(availableSize) : FSize();
		_isMeasureValid = true;
	}

	void Layoutable::ArrangeCore(FRect rect)
	{
		auto [width, height] = Shrink(FSize(rect.w, rect.h), _margin);
		
		if(width < 0)
		{
			width = 0;
		}

		if(height < 0)
		{
			height = 0;
		}

		if(_horizontalAlignment != HorizontalAlignment::Stretch)
		{
			width = std::fmin(width, _desiredSize.Width - _margin.Left - _margin.Right);
		}

		if(_verticalAlignment != VerticalAlignment::Stretch)
		{
			height = std::fmin(height, _desiredSize.Height - _margin.Top - _margin.Bottom);
		}

		width = std::clamp(width, _minWidth, _maxWidth);
		height = std::clamp(height, _minHeight, _maxHeight);

		float x = rect.x;
		float y = rect.y;

		switch(_horizontalAlignment)
		{
			case HorizontalAlignment::Right:
				x += rect.w - width;
				break;

			case HorizontalAlignment::Center:
				x += (rect.w - width) * 0.5f;
				break;

			default:
				break;
		}

		switch(_verticalAlignment)
		{
			case VerticalAlignment::Bottom:
				y += rect.h - height;
				break;

			case VerticalAlignment::Center:
				y += (rect.h - height) * 0.5f;
				break;

			default:
				break;
		}

		_bounds = FRect(x, y, width, height);
		ArrangeContent(_bounds);
	}

	FSize Layoutable::MeasureCore(FSize availableSize)
	{
		FSize constrainedSize = Shrink(availableSize, _margin);
		constrainedSize.Width = std::clamp(constrainedSize.Width, _minWidth, _maxWidth);
		constrainedSize.Height = std::clamp(constrainedSize.Height, _minHeight, _maxHeight);

		auto [contentWidth, contentHeight] = MeasureContent(constrainedSize);

		FSize desiredSize =
		{
			.Width = std::clamp(std::max(_width, contentWidth), _minWidth, _maxWidth),
			.Height = std::clamp(std::max(_height, contentHeight), _minHeight, _maxHeight)
		};

		desiredSize = Expand(desiredSize, _margin);

		if(desiredSize.Width < 0.0f)
		{
			desiredSize.Width = 0.0f;
		}

		if(desiredSize.Height < 0.0f)
		{
			desiredSize.Height = 0.0f;
		}

		return desiredSize;
	}

	void Layoutable::InvalidateArrange()
	{
		InvalidateRender();

		if(_isArrangeValid)
		{
			_isArrangeValid = false;

			if(_layotableParent)
			{
				_layotableParent->InvalidateArrange();
			}
		}
	}

	void Layoutable::InvalidateMeasure()
	{
		InvalidateRender();

		if(_isMeasureValid)
		{
			_isMeasureValid = false;
			_isArrangeValid = false;			

			if(_layotableParent)
			{
				_layotableParent->InvalidateMeasure();
			}
		}
	}

	void Layoutable::OnPropertyChanged(PropertyBase& property)
	{
		Renderable::OnPropertyChanged(property);

		if(property == WidthProperty || 
		   property == HeightProperty ||
		   property == MinWidthProperty ||
		   property == MinHeightProperty ||
		   property == MaxWidthProperty ||
		   property == MaxHeightProperty ||
		   property == MarginProperty ||
		   property == IsVisibleProperty)
		{
			InvalidateMeasure();
		}
		else if(property == VerticalAlignmentProperty || property == HorizontalAlignmentProperty)
		{
			InvalidateArrange();
		}
	}

	void Layoutable::SetParent(IStyleHost* parent)
	{
		Renderable::SetParent(parent);
		_layotableParent = dynamic_cast<Layoutable*>(parent);
	}

	void Layoutable::OnAttachedToLogicalTree()
	{
		Renderable::OnAttachedToLogicalTree();
		InvalidateMeasure();
	}

	void Layoutable::OnDetachedFromLogicalTree()
	{
		Renderable::OnDetachedFromLogicalTree();
		InvalidateMeasure();
	}
}
