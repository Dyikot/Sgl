#include "Layoutable.h"
#include <algorithm>

namespace Sgl
{
	Layoutable::Layoutable(const Layoutable& other):
		StyleableElement(other),
		_bounds(other._bounds),
		_desiredSize(other._desiredSize),
		_layoutableParent(other._layoutableParent),
		_width(other._width),
		_height(other._height),
		_minWidth(other._minWidth),
		_minHeight(other._minHeight),
		_maxWidth(other._maxWidth),
		_maxHeight(other._maxHeight),
		_isVisible(other._isVisible),
		_margin(other._margin),
		_verticalAlignment(other._verticalAlignment),
		_horizontalAlignment(other._horizontalAlignment),
		_isArrangeValid(other._isArrangeValid),
		_isMeasureValid(other._isMeasureValid)
	{}

	Layoutable::Layoutable(Layoutable&& other) noexcept:
		StyleableElement(std::move(other)),
		_bounds(other._bounds),
		_desiredSize(other._desiredSize),
		_layoutableParent(std::exchange(other._layoutableParent, nullptr)),
		_width(other._width),
		_height(other._height),
		_minWidth(other._minWidth),
		_minHeight(other._minHeight),
		_maxWidth(other._maxWidth),
		_maxHeight(other._maxHeight),
		_isVisible(other._isVisible),
		_margin(other._margin),
		_verticalAlignment(other._verticalAlignment),
		_horizontalAlignment(other._horizontalAlignment),
		_isArrangeValid(other._isArrangeValid),
		_isMeasureValid(other._isMeasureValid)
	{}

	void Layoutable::Arrange(FRect rect)
	{
		if(!_isMeasureValid)
		{
			Measure(FSize(rect.w, rect.h));
		}

		if(_isVisible)
		{
			_isArrangeValid = true;
			ArrangeCore(rect);
		}
	}

	void Layoutable::Measure(FSize avaliableSize)
	{
		if(!_isMeasureValid && _isVisible)
		{
			_isMeasureValid = true;
			_desiredSize = MeasureCore(avaliableSize);
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
		if(_isArrangeValid)
		{
			_isArrangeValid = false;

			if(_layoutableParent != nullptr)
			{
				_layoutableParent->InvalidateArrange();
			}
		}
	}

	void Layoutable::InvalidateMeasure()
	{
		if(_isMeasureValid)
		{
			_isMeasureValid = false;
			_isArrangeValid = false;

			if(_layoutableParent != nullptr)
			{
				_layoutableParent->InvalidateMeasure();
			}
		}
	}
}
