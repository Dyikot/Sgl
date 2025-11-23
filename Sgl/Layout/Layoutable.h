#pragma once

#include "../Base/Primitives.h"
#include "Layout.h"
#include "../Render/Renderable.h"

namespace Sgl
{
	class Layoutable : public Renderable
	{
	protected:
		FRect _bounds = {};
		FSize _desiredSize = {};
	private:
		float _width = 0;
		float _height = 0;
		float _minWidth = 0;
		float _minHeight = 0;
		float _maxWidth = std::numeric_limits<float>::max();
		float _maxHeight = std::numeric_limits<float>::max();
		Thickness _margin;
		bool _isVisible = true;
		VerticalAlignment _verticalAlignment = VerticalAlignment::Top;
		HorizontalAlignment _horizontalAlignment = HorizontalAlignment::Left;
		Layoutable* _layotableParent = nullptr;

		bool _isArrangeValid = false;
		bool _isMeasureValid = false;
	public:
		Layoutable() = default;
		Layoutable(const Layoutable& other);
		Layoutable(Layoutable&& other) noexcept;
		virtual ~Layoutable() = default;

		void SetWidth(float value);
		float GetWidth() const { return _width; }

		void SetHeight(float value);
		float GetHeight() const { return _height; }

		void SetMinWidth(float value);
		float GetMinWidth() const { return _minWidth; }

		void SetMinHeight(float value);
		float GetMinHeight() const { return _minHeight; }

		void SetMaxWidth(float value);
		float GetMaxWidth() const { return _maxWidth; }

		void SetMaxHeight(float value);
		float GetMaxHeight() const { return _maxHeight; }		

		void SetMargin(Thickness value);
		Thickness GetMargin() const { return _margin; }

		void SetIsVisible(bool value);
		bool IsVisible() const { return _isVisible; }

		void SetVerticalAlignment(VerticalAlignment value);
		VerticalAlignment GetVerticalAlignment() const { return _verticalAlignment; }

		void SetHorizontalAlignment(HorizontalAlignment value);
		HorizontalAlignment GetHorizontalAlignment() const { return _horizontalAlignment; }

		void SetParent(StyleableElement* parent) override;
		Layoutable* GetLayoutableParent() const { return _layotableParent; }

		FSize GetDesiredSize() const { return _desiredSize; }
		FRect GetBounds() const { return _bounds; }
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
		static inline ObservableProperty WidthProperty { &SetWidth, &GetWidth };
		static inline ObservableProperty HeightProperty { &SetHeight, &GetHeight };
		static inline ObservableProperty MinWidthProperty { &SetMinWidth, &GetMinWidth };
		static inline ObservableProperty MinHeightProperty { &SetMinHeight, &GetMinHeight };
		static inline ObservableProperty MaxWidthProperty { &SetMaxWidth, &GetMaxWidth };
		static inline ObservableProperty MaxHeightProperty { &SetMaxHeight, &GetMaxHeight };
		static inline ObservableProperty MarginProperty { &SetMargin, &GetMargin };
		static inline ObservableProperty IsVisibleProperty { &SetIsVisible, &IsVisible };
		static inline ObservableProperty VerticalAlignmentProperty { &SetVerticalAlignment, &GetVerticalAlignment };
		static inline ObservableProperty HorizontalAlignmentProperty { &SetHorizontalAlignment, &GetHorizontalAlignment };
	};
}