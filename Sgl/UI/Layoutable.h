#pragma once

#include "../Base/Layout.h"
#include "../Base/Size.h"
#include "Style/StyleableElement.h"
#include "../Base/Primitives.h"

namespace Sgl
{
	class Layoutable: public StyleableElement
	{
	protected:
		FRect _bounds;
		FSize _desiredSize;
		Layoutable* _layoutableParent;
	private:
		float _width;
		float _height;
		float _minWidth;
		float _minHeight;
		float _maxWidth;
		float _maxHeight;
		bool _isVisible;
		Thickness _margin;
		VerticalAlignment _verticalAlignment;
		HorizontalAlignment _horizontalAlignment;
		bool _isArrangeValid;
		bool _isMeasureValid;
	public:
		Layoutable();
		Layoutable(const Layoutable& other);
		Layoutable(Layoutable&& other) noexcept;
		virtual ~Layoutable() = default;

		void SetWidth(float value) 
		{
			SetProperty(WidthProperty, _width, value);
			InvalidateMeasure();
		}

		float GetWidth() const 
		{
			return _width;
		}

		void SetHeight(float value)
		{ 
			SetProperty(HeightProperty, _height, value);
			InvalidateMeasure();
		}

		float GetHeight() const 
		{
			return _height;
		}

		void SetMinWidth(float value)
		{ 
			SetProperty(MinWidthProperty, _minWidth, value);
			InvalidateMeasure();
		}

		float GetMinWidth() const 
		{
			return _minWidth;
		}

		void SetMinHeight(float value)
		{
			SetProperty(MinHeightProperty, _minHeight, value);
			InvalidateMeasure();
		}

		float GetMinHeight() const 
		{
			return _minHeight; 
		}

		void SetMaxWidth(float value) 
		{
			SetProperty(MaxWidthProperty, _maxWidth, value); 
			InvalidateMeasure();
		}

		float GetMaxWidth() const
		{ 
			return _maxWidth; 
		}

		void SetMaxHeight(float value)
		{ 
			SetProperty(MaxHeightProperty, _maxHeight, value);
			InvalidateMeasure();
		}

		float GetMaxHeight() const
		{ 
			return _maxHeight;
		}

		void SetIsVisible(bool value)
		{ 
			SetProperty(IsVisibleProperty, _isVisible, value);
			InvalidateMeasure();
		}

		bool IsVisible() const
		{
			return _isVisible;
		}

		void SetMargin(Thickness value)
		{ 
			SetProperty(MarginProperty, _margin, value);
			InvalidateMeasure();
		}

		Thickness GetMargin() const 
		{ 
			return _margin; 
		}

		void SetVerticalAlignment(VerticalAlignment value)
		{ 
			SetProperty(VerticalAlignmentProperty, _verticalAlignment, value);
			InvalidateArrange();
		}

		VerticalAlignment GetVerticalAlignment() const
		{ 
			return _verticalAlignment;
		}

		void SetHorizontalAlignment(HorizontalAlignment value)
		{ 
			SetProperty(HorizontalAlignmentProperty, _horizontalAlignment, value);
			InvalidateArrange();
		}

		HorizontalAlignment GetHorizontalAlignment() const
		{ 
			return _horizontalAlignment; 
		}

		FSize GetDesiredSize() const { return _desiredSize; }
		FRect GetBounds() const { return _bounds; }
		Layoutable* GetLayoutableParent() const { return _layoutableParent; }
		bool IsArrangeValid() const { return _isArrangeValid; }
		bool IsMeasureValid() const { return _isMeasureValid; }

		virtual void Arrange(FRect rect);
		virtual void Measure(FSize avaliableSize);
		void InvalidateArrange();
		void InvalidateMeasure();
	protected:
		virtual FSize MeasureContent(FSize avaliableSize) { return FSize(); }
		virtual void ArrangeContent(FRect rect) { }
	public:
		static inline BindableProperty<Layoutable, float> WidthProperty =
			BindableProperty<Layoutable, float>(&SetWidth);

		static inline BindableProperty<Layoutable, float> HeightProperty =
			BindableProperty<Layoutable, float>(&SetHeight);

		static inline BindableProperty<Layoutable, float> MinWidthProperty =
			BindableProperty<Layoutable, float>(&SetMinWidth);

		static inline BindableProperty<Layoutable, float> MinHeightProperty =
			BindableProperty<Layoutable, float>(&SetMinHeight);

		static inline BindableProperty<Layoutable, float> MaxWidthProperty =
			BindableProperty<Layoutable, float>(&SetMaxWidth, std::numeric_limits<float>::max());

		static inline BindableProperty<Layoutable, float> MaxHeightProperty =
			BindableProperty<Layoutable, float>(&SetMaxHeight, std::numeric_limits<float>::max());

		static inline BindableProperty<Layoutable, bool> IsVisibleProperty =
			BindableProperty<Layoutable, bool>(&SetIsVisible, true);

		static inline BindableProperty<Layoutable, Thickness> MarginProperty =
			BindableProperty<Layoutable, Thickness>(&SetMargin);

		static inline BindableProperty<Layoutable, VerticalAlignment> VerticalAlignmentProperty =
			BindableProperty<Layoutable, VerticalAlignment>(&SetVerticalAlignment, VerticalAlignment::Top);

		static inline BindableProperty<Layoutable, HorizontalAlignment> HorizontalAlignmentProperty =
			BindableProperty<Layoutable, HorizontalAlignment>(&SetHorizontalAlignment, HorizontalAlignment::Left);
	};
}