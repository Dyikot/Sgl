#include "Layoutable.h"
#include <algorithm>

namespace Sgl
{
	Layoutable::Layoutable():
		StyleableElement(),
		Width(*this),
		Height(*this),
		MinWidth(*this),
		MinHeight(*this),
		MaxWidth(*this, std::numeric_limits<float>::max()),
		MaxHeight(*this, std::numeric_limits<float>::max()),
		IsVisible(*this, true),
		Margin(*this),
		VerticalAlignment(*this, VerticalAlignment::Top),
		HorizontalAlignment(*this, HorizontalAlignment::Left),
		_bounds(),
		_desiredSize(),
		_layoutableParent(nullptr),
		_isArrangeValid(false),
		_isMeasureValid(false)
	{}

	Layoutable::Layoutable(const Layoutable& other):
		StyleableElement(other),
		Width(other.Width),
		Height(other.Height),
		MinWidth(other.MinWidth),
		MinHeight(other.MinHeight),
		MaxWidth(other.MaxWidth),
		MaxHeight(other.MaxHeight),
		IsVisible(other.IsVisible),
		Margin(other.Margin),
		VerticalAlignment(other.VerticalAlignment),
		HorizontalAlignment(other.HorizontalAlignment),
		_bounds(other._bounds),
		_desiredSize(other._desiredSize),
		_layoutableParent(other._layoutableParent),
		_isArrangeValid(other._isArrangeValid),
		_isMeasureValid(other._isMeasureValid)
	{}

	Layoutable::Layoutable(Layoutable&& other) noexcept:
		StyleableElement(std::move(other)),
		Width(other.Width),
		Height(other.Height),
		MinWidth(other.MinWidth),
		MinHeight(other.MinHeight),
		MaxWidth(other.MaxWidth),
		MaxHeight(other.MaxHeight),
		IsVisible(other.IsVisible),
		Margin(other.Margin),
		VerticalAlignment(other.VerticalAlignment),
		HorizontalAlignment(other.HorizontalAlignment),
		_bounds(other._bounds),
		_desiredSize(other._desiredSize),
		_layoutableParent(std::exchange(other._layoutableParent, nullptr)),
		_isArrangeValid(other._isArrangeValid),
		_isMeasureValid(other._isMeasureValid)
	{}

	void Layoutable::Arrange(FRect rect)
	{
		if(!_isMeasureValid)
		{
			Measure(FSize(rect.w, rect.h));
		}

		if(IsVisible)
		{
			Thickness margin = Margin;
			auto x = rect.x + margin.Left;
			auto y = rect.y + margin.Top;

			auto availableWidth = rect.w - margin.Left - margin.Right;
			auto availableHeight = rect.h - margin.Top - margin.Bottom;

			if(availableWidth < 0)
			{
				availableWidth = 0;
			}

			if(availableHeight < 0)
			{
				availableHeight = 0;
			}

			auto horizontalAlignment = HorizontalAlignment.Get();
			auto verticalAlignment = VerticalAlignment.Get();
			auto width = availableWidth;
			auto height = availableHeight;

			if(horizontalAlignment != HorizontalAlignment::Stretch)
			{
				width = std::fmin(width, _desiredSize.Width - margin.Left - margin.Right);
			}

			if(verticalAlignment != VerticalAlignment::Stretch)
			{
				height = std::fmin(height, _desiredSize.Height - margin.Top - margin.Bottom);
			}

			width = std::clamp<float>(width, MinWidth, MaxWidth);
			height = std::clamp<float>(height, MinHeight, MaxHeight);

			switch(horizontalAlignment)
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

			switch(verticalAlignment)
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
			_isArrangeValid = true;
		}
	}

	void Layoutable::Measure(FSize avaliableSize)
	{
		if(!_isMeasureValid && IsVisible)
		{
			Thickness margin = Margin;

			FSize contentAvaliableSize = 
			{
				.Width = std::clamp<float>(avaliableSize.Width - margin.Left - margin.Right,
					MinWidth,
					MaxWidth),
				.Height = std::clamp<float>(avaliableSize.Height - margin.Top - margin.Bottom,
					MinHeight,
					MaxHeight)
			};

			auto [contentWidth, contentHeight] = MeasureContent(contentAvaliableSize);

			float width = std::clamp<float>(std::fmax(Width, contentWidth), MinWidth, MaxWidth);
			float height = std::clamp<float>(std::fmax(Height, contentHeight), MinHeight, MaxHeight);

			width += margin.Left + margin.Right;
			height += margin.Top + margin.Bottom;

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

			_desiredSize = FSize(width, height);
			_isMeasureValid = true;
		}
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
