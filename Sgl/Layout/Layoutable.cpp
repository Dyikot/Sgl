#include "Layoutable.h"

#include <algorithm>
#include "../Base/Logger.h"

namespace Sgl
{
	Layoutable::Layoutable(const Layoutable& other):
		Renderable(other),
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
	{}

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
	{}

	void Layoutable::SetWidth(float value)
	{
		SetWidth(value, ValueSource::Local);
	}

	void Layoutable::SetWidth(float value, ValueSource source)
	{
		if(SetProperty(WidthProperty, _width, value, _widthSource, source))
		{
			InvalidateMeasure();
		}
	}

	void Layoutable::SetHeight(float value)
	{
		SetHeight(value, ValueSource::Local);
	}

	void Layoutable::SetHeight(float value, ValueSource source)
	{
		if(SetProperty(HeightProperty, _height, value, _heightSource, source))
		{
			InvalidateMeasure();
		}
	}

	void Layoutable::SetMinWidth(float value)
	{
		SetMinWidth(value, ValueSource::Local);
	}

	void Layoutable::SetMinWidth(float value, ValueSource source)
	{
		if(SetProperty(MinWidthProperty, _minWidth, value, _minWidthSource, source))
		{
			InvalidateMeasure();
		}
	}

	void Layoutable::SetMinHeight(float value)
	{
		SetMinHeight(value, ValueSource::Local);
	}

	void Layoutable::SetMinHeight(float value, ValueSource source)
	{
		if(SetProperty(MinHeightProperty, _minHeight, value, _minHeightSource, source))
		{
			InvalidateMeasure();
		}
	}

	void Layoutable::SetMaxWidth(float value)
	{
		SetMaxWidth(value, ValueSource::Local);
	}

	void Layoutable::SetMaxWidth(float value, ValueSource source)
	{
		if(SetProperty(MaxWidthProperty, _maxWidth, value, _maxWidthSource, source))
		{
			InvalidateMeasure();
		}
	}

	void Layoutable::SetMaxHeight(float value)
	{
		SetMaxHeight(value, ValueSource::Local);
	}

	void Layoutable::SetMaxHeight(float value, ValueSource source)
	{
		if(SetProperty(MaxHeightProperty, _maxHeight, value, _maxHeightSource, source))
		{
			InvalidateMeasure();
		}
	}

	void Layoutable::SetMargin(Thickness value)
	{
		SetMargin(value, ValueSource::Local);
	}

	void Layoutable::SetMargin(Thickness value, ValueSource source)
	{
		if(SetProperty(MarginProperty, _margin, value, _marginSource, source))
		{
			InvalidateMeasure();
		}
	}

	void Layoutable::SetIsVisible(bool value)
	{
		SetIsVisible(value, ValueSource::Local);
	}

	void Layoutable::SetIsVisible(bool value, ValueSource source)
	{
		if(SetProperty(IsVisibleProperty, _isVisible, value, _isVisibleSource, source))
		{
			InvalidateMeasure();
		}
	}

	void Layoutable::SetVerticalAlignment(VerticalAlignment value)
	{
		SetVerticalAlignment(value, ValueSource::Local);
	}

	void Layoutable::SetVerticalAlignment(VerticalAlignment value, ValueSource source)
	{
		if(SetProperty(VerticalAlignmentProperty, _verticalAlignment, value, _verticalAlignmentSource, source))
		{
			InvalidateArrange();
		}
	}

	void Layoutable::SetHorizontalAlignment(HorizontalAlignment value)
	{
		SetHorizontalAlignment(value, ValueSource::Local);
	}

	void Layoutable::SetHorizontalAlignment(HorizontalAlignment value, ValueSource source)
	{
		if(SetProperty(HorizontalAlignmentProperty, _horizontalAlignment, value, _horizontalAlignmentSource, source))
		{
			InvalidateArrange();
		}
	}

	void Layoutable::SetParent(IStyleHost* parent)
	{
		Renderable::SetParent(parent);

		if(parent == nullptr)
		{
			_layotableParent = nullptr;
			return;
		}

		if(auto layoutableParent = dynamic_cast<Layoutable*>(parent))
		{
			_layotableParent = layoutableParent;
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

	void Layoutable::Measure(FSize avaliableSize)
	{
		if(!_isMeasureValid)
		{
			_isMeasureValid = true;
			_desiredSize = IsVisible() ? MeasureCore(avaliableSize) : FSize();
		}
	}

	void Layoutable::ArrangeCore(FRect rect)
	{
		auto x = rect.x + _margin.Left;
		auto y = rect.y + _margin.Top;

		auto availableWidth = rect.w - _margin.Left - _margin.Right;
		auto availableHeight = rect.h - _margin.Top - _margin.Bottom;

		if(availableWidth < 0)
		{
			availableWidth = 0;
		}

		if(availableHeight < 0)
		{
			availableHeight = 0;
		}

		auto width = availableWidth;
		auto height = availableHeight;

		if(_horizontalAlignment != HorizontalAlignment::Stretch)
		{
			width = std::fmin(width, _desiredSize.Width - _margin.Left - _margin.Right);
		}

		if(_verticalAlignment != VerticalAlignment::Stretch)
		{
			height = std::fmin(height, _desiredSize.Height - _margin.Top - _margin.Bottom);
		}

		width = std::clamp<float>(width, _minWidth, _maxWidth);
		height = std::clamp<float>(height, _minHeight, _maxHeight);

		switch(_horizontalAlignment)
		{
			case HorizontalAlignment::Right:
				x += availableWidth - width;
				break;

			case HorizontalAlignment::Center:
				x += (availableWidth - width) / 2.f;
				break;

			default:
				break;
		}

		switch(_verticalAlignment)
		{
			case VerticalAlignment::Bottom:
				y += availableHeight - height;
				break;

			case VerticalAlignment::Center:
				y += (availableHeight - height) / 2.f;
				break;

			default:
				break;
		}

		_bounds = FRect(x, y, width, height);
		ArrangeContent(_bounds);
	}

	FSize Layoutable::MeasureCore(FSize avaliableSize)
	{
		FSize contentAvaliableSize =
		{
			.Width = std::clamp<float>(avaliableSize.Width - _margin.Left - _margin.Right,
				_minWidth,
				_maxWidth),
			.Height = std::clamp<float>(avaliableSize.Height - _margin.Top - _margin.Bottom,
				_minHeight,
				_maxHeight)
		};

		auto [contentWidth, contentHeight] = MeasureContent(contentAvaliableSize);

		float width = std::clamp<float>(std::fmax(_width, contentWidth), _minWidth, _maxWidth);
		float height = std::clamp<float>(std::fmax(_height, contentHeight), _minHeight, _maxHeight);

		width += _margin.Left + _margin.Right;
		height += _margin.Top + _margin.Bottom;

		if(width > avaliableSize.Width)
		{
			width = avaliableSize.Width;
		}
		else if(width < 0)
		{
			width = 0;
		}

		if(height > avaliableSize.Height)
		{
			height = avaliableSize.Height;
		}
		else if(height < 0)
		{
			height = 0;
		}

		return FSize(width, height);
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
}
