#pragma once

#include "RangeBase.h"

namespace Sgl::UIElements
{
	class ScrollBar : public RangeBase
	{
	public:
		ScrollBar();
		ScrollBar(ScrollBar&& other) noexcept;

		void SetViewportSize(float value, ValueSource source = ValueSource::Local);
		float GetViewportSize() const { return _viewportSize; }

		void SetSmallChange(float value, ValueSource source = ValueSource::Local);
		float GetSmallChange() const { return _smallChange; }

		void SetLargeChange(float value, ValueSource source = ValueSource::Local);
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
		void OnValueChanged(float value) override;
		void OnMouseDown(MouseClickEventArgs& e) override;
	private:
		void BuildTemplate();
		float GetPageSize() const;
	private:
		class Track;

		Ref<UIElement> _thumb;
		Ref<Track> _track;

		float _dragValue = 0;
		float _clickValue = 0;
		float _viewportSize = 0;
		float _scrollScale = 0;
		float _smallChange = 1;
		float _largeChange = 10;

		ValueSource _viewportSizeSource {};
		ValueSource _smallChangeSource {};
		ValueSource _largeChangeSource {};
		bool _wasHoverThumb = false;
	};
}