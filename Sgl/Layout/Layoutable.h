#pragma once

#include "../Base/Primitives.h"
#include "../Render/Renderable.h"
#include "Layout.h"

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

		ValueSource _widthSource {};
		ValueSource _heightSource {};
		ValueSource _minWidthSource {};
		ValueSource _minHeightSource {};
		ValueSource _maxWidthSource {};
		ValueSource _maxHeightSource {};
		ValueSource _marginSource {};
		ValueSource _isVisibleSource {};
		ValueSource _verticalAlignmentSource {};
		ValueSource _horizontalAlignmentSource {};
	public:
		Layoutable() = default;
		Layoutable(const Layoutable& other);
		Layoutable(Layoutable&& other) noexcept;
		virtual ~Layoutable() = default;

		void SetWidth(float value, ValueSource source = ValueSource::Local);
		float GetWidth() const { return _width; }

		void SetHeight(float value, ValueSource source = ValueSource::Local);
		float GetHeight() const { return _height; }

		void SetMinWidth(float value, ValueSource source = ValueSource::Local);
		float GetMinWidth() const { return _minWidth; }

		void SetMinHeight(float value, ValueSource source = ValueSource::Local);
		float GetMinHeight() const { return _minHeight; }

		void SetMaxWidth(float value, ValueSource source = ValueSource::Local);
		float GetMaxWidth() const { return _maxWidth; }

		void SetMaxHeight(float value, ValueSource source = ValueSource::Local);
		float GetMaxHeight() const { return _maxHeight; }		

		void SetMargin(Thickness value, ValueSource source = ValueSource::Local);
		Thickness GetMargin() const { return _margin; }

		void SetIsVisible(bool value, ValueSource source = ValueSource::Local);
		bool IsVisible() const { return _isVisible; }

		void SetVerticalAlignment(VerticalAlignment value, ValueSource source = ValueSource::Local);
		VerticalAlignment GetVerticalAlignment() const { return _verticalAlignment; }

		void SetHorizontalAlignment(HorizontalAlignment value, ValueSource source = ValueSource::Local);
		HorizontalAlignment GetHorizontalAlignment() const { return _horizontalAlignment; }

		void SetParent(IStyleHost* parent) override;
		Layoutable* GetLayoutableParent() const { return _layotableParent; }

		FSize GetDesiredSize() const { return _desiredSize; }
		FRect GetBounds() const { return _bounds; }
		bool NeedsArrange() const { return !_isArrangeValid; }
		bool NeedsMeasure() const { return !_isMeasureValid; }

		void Arrange(FRect rect);
		void Measure(FSize avaliableSize);
		virtual void ArrangeCore(FRect rect);
		virtual FSize MeasureCore(FSize avaliableSize);
		void InvalidateArrange();
		void InvalidateMeasure();
	protected:
		virtual FSize MeasureContent(FSize avaliableSize) { return FSize(); }
		virtual void ArrangeContent(FRect rect) { }
		void OnAttachedToLogicalTree() override;
		void OnDetachedFromLogicalTree() override;
	public:
		static inline StyleableProperty WidthProperty { &SetWidth, &GetWidth };
		static inline StyleableProperty HeightProperty { &SetHeight, &GetHeight };
		static inline StyleableProperty MinWidthProperty { &SetMinWidth, &GetMinWidth };
		static inline StyleableProperty MinHeightProperty { &SetMinHeight, &GetMinHeight };
		static inline StyleableProperty MaxWidthProperty { &SetMaxWidth, &GetMaxWidth };
		static inline StyleableProperty MaxHeightProperty { &SetMaxHeight, &GetMaxHeight };
		static inline StyleableProperty MarginProperty { &SetMargin, &GetMargin };
		static inline StyleableProperty IsVisibleProperty { &SetIsVisible, &IsVisible };
		static inline StyleableProperty VerticalAlignmentProperty { &SetVerticalAlignment, &GetVerticalAlignment };
		static inline StyleableProperty HorizontalAlignmentProperty { &SetHorizontalAlignment, &GetHorizontalAlignment };
	};
}