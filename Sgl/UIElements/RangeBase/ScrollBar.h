#pragma once

#include "RangeBase.h"

namespace Sgl::UIElements
{
	class RepeatButton;

	class ScrollBar : public RangeBase
	{
	public:
		ScrollBar();

		void SetViewportSize(float value);
		float GetViewportSize() const { return _viewportSize; }

		void SetSmallChange(float value);
		float GetSmallChange() const { return _smallChange; }

		void SetLargeChange(float value);
		float GetLargeChange() const { return _largeChange; }

		void LineUp();
		void LineDown();
		void PageUp();
		void PageDown();
		void ScrollToHome();
		void ScrollToEnd();

		static inline StyleableProperty ViewportSizeProperty { &SetViewportSize, &GetViewportSize };
		static inline StyleableProperty SmallChangeProperty { &SetSmallChange, &GetSmallChange };
		static inline StyleableProperty LargeChangeProperty { &SetLargeChange, &GetLargeChange };
	protected:
		void OnPropertyChanged(PropertyBase& property) override;
		void OnValueChanged(float value) override;
		Ref<Style> CreateDefaultStyle() const override;
	private:
		void BuildTemplate();
		float GetPageSize() const;
	private:
		Ref<UIElement> _thumb;
		Ref<RepeatButton> _track;

		float _dragValue = 0;
		float _clickValue = 0;
		float _viewportSize = 0;
		float _scrollScale = 0;
		float _smallChange = 1;
		float _largeChange = 10;

		bool _wasHoverThumb = false;
		ValueSource _viewportSizeSource {};
		ValueSource _smallChangeSource {};
		ValueSource _largeChangeSource {};
	};

	class ScrollBarStyle : public UIElementStyle
	{
	public:
		void OnBase(StyleContext& context) override;
	};
}