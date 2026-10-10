#pragma once

#include "UIElement.h"

namespace Sgl
{
	class ContentUIElement;
}

namespace Sgl::UIElements
{
	class ScrollBar;

	struct ScrollInfo
	{
		float VerticalOffset;
		float HorizontalOffset;
		float MaxVerticalOffset;
		float MaxHorizontalOffset;
		FSize Viewport;
		FSize Extent;
	};

	class ScrollableView : public UIElement
	{
	public:
		using ScrollEvenetHandler = EventHandler<ScrollableView>;
	public:
		ScrollableView();

		Event<ScrollEvenetHandler> ScrollMeasured;
		Event<ScrollEvenetHandler> ScrollChanged;

		void SetContent(const Ref<ObservableObject>& value, ValueSource source = ValueSource::Local);
		const Ref<ObservableObject>& GetContent() const;

		void SetContentTemplate(const Ref<IDataTemplate>& value, ValueSource source = ValueSource::Local);
		const Ref<IDataTemplate>& GetContentTemplate() const;

		void SetScrollStep(float value, ValueSource source = ValueSource::Local);
		float GetScrollStep() const { return _scrollStep; }

		void SetHorizontalOffset(float value);
		float GetHorizontalOffset() const { return _scrollInfo.HorizontalOffset; }

		void SetVerticalOffset(float value);
		float GetVerticalOffset() const { return _scrollInfo.VerticalOffset; }

		float GetMaxHorizontalOffset() const { return _scrollInfo.MaxHorizontalOffset; }
		float GetMaxVerticalOffset() const { return _scrollInfo.MaxVerticalOffset; }

		FSize GetExtent() const { return _scrollInfo.Extent; }
		FSize GetViewport() const { return _scrollInfo.Viewport; }

		void LineUp();
		void LineDown();
		void LineLeft();
		void LineRight();

		void ScrollToTop();
		void ScrollToBottom();
		void ScrollToLeft();
		void ScrollToRight();

		static inline StyleableProperty ContentProperty { &SetContent, &GetContent };
		static inline StyleableProperty ContentTemplateProperty { &SetContentTemplate, &GetContentTemplate };
		static inline StyleableProperty ScrollStepProperty { &SetScrollStep, &GetScrollStep };
	protected:
		void OnKeyDown(KeyEventArgs& e) override;
		void OnMouseWheelChanged(MouseWheelEventArgs& e) override;
		FSize MeasureCore(FSize availableSize) override;
		FSize MeasureContent(FSize availableSize) override;
		void ArrangeContent(FRect rect) override;
	private:
		void BuildTemplate();
	private:
		ScrollInfo _scrollInfo {};
		float _scrollStep = 40;

		Ref<ScrollBar> _verticalScrollBar;
		Ref<ScrollBar> _horizontalScrollBar;
		Ref<ContentUIElement> _content;

		ValueSource _ScrollStepSource {};
	};
}