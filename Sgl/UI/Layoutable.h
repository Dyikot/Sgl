#pragma once

#include "../Base/Media/Layout.h"
#include "../Base/Size.h"
#include "Style/StyleableElement.h"
#include "../Base/Primitives.h"

namespace Sgl
{
	class Layoutable: public StyleableElement
	{
	protected:
		FRect _bounds {};
		FSize _desiredSize {};
		Layoutable* _layoutableParent {};
	private:
		float _width {};
		float _height {};
		float _minWidth {};
		float _minHeight {};
		float _maxWidth = std::numeric_limits<float>::max();
		float _maxHeight = std::numeric_limits<float>::max();
		bool _isVisible = true;
		Thickness _margin;
		VerticalAlignment _verticalAlignment = VerticalAlignment::Top;
		HorizontalAlignment _horizontalAlignment = HorizontalAlignment::Left;
		bool _isArrangeValid = false;
		bool _isMeasureValid = false;
	public:
		Layoutable() = default;
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

		void Arrange(FRect rect);
		void Measure(FSize avaliableSize);
		virtual void ArrangeCore(FRect rect);
		virtual FSize MeasureCore(FSize avaliableSize);
		void InvalidateArrange();
		void InvalidateMeasure();
	protected:
		virtual FSize MeasureContent(FSize avaliableSize) { return FSize(); }
		virtual void ArrangeContent(FRect rect) { }
	public:
		static inline ObservableProperty<Layoutable, float> WidthProperty =
			ObservableProperty<Layoutable, float>(&SetWidth);

		static inline ObservableProperty<Layoutable, float> HeightProperty =
			ObservableProperty<Layoutable, float>(&SetHeight);

		static inline ObservableProperty<Layoutable, float> MinWidthProperty =
			ObservableProperty<Layoutable, float>(&SetMinWidth);

		static inline ObservableProperty<Layoutable, float> MinHeightProperty =
			ObservableProperty<Layoutable, float>(&SetMinHeight);

		static inline ObservableProperty<Layoutable, float> MaxWidthProperty =
			ObservableProperty<Layoutable, float>(&SetMaxWidth);

		static inline ObservableProperty<Layoutable, float> MaxHeightProperty =
			ObservableProperty<Layoutable, float>(&SetMaxHeight);

		static inline ObservableProperty<Layoutable, bool> IsVisibleProperty =
			ObservableProperty<Layoutable, bool>(&SetIsVisible);

		static inline ObservableProperty<Layoutable, Thickness> MarginProperty =
			ObservableProperty<Layoutable, Thickness>(&SetMargin);

		static inline ObservableProperty<Layoutable, VerticalAlignment> VerticalAlignmentProperty =
			ObservableProperty<Layoutable, VerticalAlignment>(&SetVerticalAlignment);

		static inline ObservableProperty<Layoutable, HorizontalAlignment> HorizontalAlignmentProperty =
			ObservableProperty<Layoutable, HorizontalAlignment>(&SetHorizontalAlignment);
	};
}