#include "Layoutable.h"

#include <algorithm>
#include "../Base/Logging.h"
#include "LayoutHelper.h"

namespace Sgl
{
	Layoutable::Layoutable(Layoutable&& other) noexcept:
		Renderable(std::move(other)),
		_bounds(other._bounds),
		_desiredSize(other._desiredSize),
		_width(other._width),
		_height(other._height),
		_minWidth(other._minWidth),
		_minHeight(other._minHeight),
		_maxWidth(other._maxWidth),
		_maxHeight(other._maxHeight),
		_margin(other._margin),
		_isVisible(other._isVisible),
		_verticalAlignment(other._verticalAlignment),
		_horizontalAlignment(other._horizontalAlignment),
		_isArrangeValid(other._isArrangeValid),
		_isMeasureValid(other._isMeasureValid)
	{
		std::memcpy(_layoutContext, other._layoutContext, sizeof(_layoutContext));
	}

	void Layoutable::SetWidth(float value, ValueSource source)
	{
		if(SetProperty(WidthProperty, _width, value, _widthSource, source))
		{
			InvalidateMeasure();
		}
	}

	void Layoutable::SetHeight(float value, ValueSource source)
	{
		if(SetProperty(HeightProperty, _height, value, _heightSource, source))
		{
			InvalidateMeasure();
		}
	}

	void Layoutable::SetMinWidth(float value, ValueSource source)
	{
		if(SetProperty(MinWidthProperty, _minWidth, value, _minWidthSource, source))
		{
			InvalidateMeasure();
		}
	}

	void Layoutable::SetMinHeight(float value, ValueSource source)
	{
		if(SetProperty(MinHeightProperty, _minHeight, value, _minHeightSource, source))
		{
			InvalidateMeasure();
		}
	}

	void Layoutable::SetMaxWidth(float value, ValueSource source)
	{
		if(SetProperty(MaxWidthProperty, _maxWidth, value, _maxWidthSource, source))
		{
			InvalidateMeasure();
		}
	}

	void Layoutable::SetMaxHeight(float value, ValueSource source)
	{
		if(SetProperty(MaxHeightProperty, _maxHeight, value, _maxHeightSource, source))
		{
			InvalidateMeasure();
		}
	}

	void Layoutable::SetMargin(Thickness value, ValueSource source)
	{
		if(SetProperty(MarginProperty, _margin, value, _marginSource, source))
		{
			InvalidateMeasure();
		}
	}

	void Layoutable::SetIsVisible(bool value, ValueSource source)
	{
		if(SetProperty(IsVisibleProperty, _isVisible, value, _isVisibleSource, source))
		{
			InvalidateMeasure();
		}
	}

	void Layoutable::SetVerticalAlignment(VerticalAlignment value, ValueSource source)
	{
		if(SetProperty(VerticalAlignmentProperty, _verticalAlignment, value, _verticalAlignmentSource, source))
		{
			InvalidateArrange();
		}
	}

	void Layoutable::SetHorizontalAlignment(HorizontalAlignment value, ValueSource source)
	{
		if(SetProperty(HorizontalAlignmentProperty, _horizontalAlignment, value, _horizontalAlignmentSource, source))
		{
			InvalidateArrange();
		}
	}

	void Layoutable::Arrange(FRect rect)
	{
		if(!_isMeasureValid)
		{
			Measure(FSize(rect.w, rect.h));
		}

		_isArrangeValid = true;

		if(IsVisible())
		{
			ArrangeCore(rect);
		}
	}

	void Layoutable::Measure(FSize availableSize)
	{
		if(!_isMeasureValid)
		{
			_isMeasureValid = true;
			_desiredSize = IsVisible() ? MeasureCore(availableSize) : FSize();
		}
	}

	void Layoutable::ArrangeCore(FRect rect)
	{
		rect = Shrink(rect, _margin);
		rect.w = std::max(0.0f, rect.w);
		rect.h = std::max(0.0f, rect.h);

		float width = _horizontalAlignment == HorizontalAlignment::Stretch
			? rect.w
			: std::fmin(rect.w, _desiredSize.Width - _margin.Left - _margin.Right);

		float height = _verticalAlignment == VerticalAlignment::Stretch
			? rect.h
			: std::fmin(rect.h, _desiredSize.Height - _margin.Top - _margin.Bottom);

		width = std::clamp(width, _minWidth, _maxWidth);
		height = std::clamp(height, _minHeight, _maxHeight);

		float offsetX = std::max(0.0f, rect.w - width);
		float offsetY = std::max(0.0f, rect.h - height);

		switch(_horizontalAlignment)
		{
			case HorizontalAlignment::Right:
				rect.x += offsetX;
				break;

			case HorizontalAlignment::Center:
				rect.x += offsetX * 0.5f;
				break;

			default:
				break;
		}

		switch(_verticalAlignment)
		{
			case VerticalAlignment::Bottom:
				rect.y += offsetY;
				break;

			case VerticalAlignment::Center:
				rect.y += offsetY * 0.5f;
				break;

			default:
				break;
		}

		_bounds = FRect(rect.x, rect.y, width, height);
		ArrangeContent(_bounds);
	}

	FSize Layoutable::MeasureCore(FSize availableSize)
	{
		FSize contentAvailableSize = Shrink(availableSize, _margin);
		contentAvailableSize.Width = std::max(0.0f, contentAvailableSize.Width);
		contentAvailableSize.Height = std::max(0.0f, contentAvailableSize.Height);

		auto [contentWidth, contentHeight] = MeasureContent(contentAvailableSize);

		float width = std::clamp(std::fmax(_width, contentWidth), _minWidth, _maxWidth);
		float height = std::clamp(std::fmax(_height, contentHeight), _minHeight, _maxHeight);

		FSize contentSize = Expand(FSize(width, height), _margin);
		contentSize.Width = std::max(0.0f, contentSize.Width);
		contentSize.Height = std::max(0.0f, contentSize.Height);

		return contentSize;
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

	void Layoutable::SetParent(IStyleHost* parent)
	{
		Renderable::SetParent(parent);
		_layotableParent = dynamic_cast<Layoutable*>(parent);
	}

	void Layoutable::OnAttachedToLogicalTree()
	{
		Styleable::OnAttachedToLogicalTree();
		InvalidateMeasure();
	}

	void Layoutable::OnDetachedFromLogicalTree()
	{
		Styleable::OnDetachedFromLogicalTree();
		InvalidateMeasure();
	}
}
