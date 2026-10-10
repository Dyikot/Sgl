#include "ScrollableView.h"

#include "../Base/Logging.h"

#include "ContentUIElement.h"
#include "RangeBase/ScrollBar.h"

namespace Sgl::UIElements
{
	namespace
	{
		class ScrollContentPresenter : public ContentUIElement
		{
		public:
			ScrollContentPresenter(ScrollInfo& scrollInfo):
				_scrollInfo(scrollInfo)
			{
				SetName("ScrollContentPresenter");
				SetClipToBounds(true);
			}

		protected:
			void ArrangeContent(FRect rect) override
			{
				rect.x -= _scrollInfo.HorizontalOffset;
				rect.y -= _scrollInfo.VerticalOffset;
				rect.w = std::max(rect.w, _scrollInfo.Extent.Width);
				rect.h = std::max(rect.h, _scrollInfo.Extent.Height);
				ContentUIElement::ArrangeContent(rect);
			}
		private:
			ScrollInfo& _scrollInfo;
		};
	}

	ScrollableView::ScrollableView()
	{
		SetName("ScrollableView");
		SetIsFocusable(true, ValueSource::Default);
		BuildTemplate();

		_verticalScrollBar->ValueChanged += [this](RangeBase& sender, float value)
		{
			SetVerticalOffset(value);
		};

		_horizontalScrollBar->ValueChanged += [this](RangeBase& sender, float value)
		{
			SetHorizontalOffset(value);
		};

		ScrollMeasured += [this](ScrollableView& sender, EventArgs e)
		{
			auto extent = sender.GetExtent();
			auto viewport = sender.GetViewport();
			auto maxVerticalOffset = sender.GetMaxVerticalOffset();
			auto maxHorizontalOffset = sender.GetMaxHorizontalOffset();

			_verticalScrollBar->SetViewportSize(viewport.Height / extent.Height);
			_verticalScrollBar->SetMaxValue(maxVerticalOffset);

			_horizontalScrollBar->SetViewportSize(viewport.Width / extent.Width);
			_horizontalScrollBar->SetMaxValue(maxHorizontalOffset);
		};

		ScrollChanged += [this](ScrollableView& sender, EventArgs e)
		{
			auto verticalOffset = sender.GetVerticalOffset();
			auto horizontalOffset = sender.GetHorizontalOffset();

			_verticalScrollBar->SetValue(verticalOffset);
			_horizontalScrollBar->SetValue(horizontalOffset);
		};
	}

	void ScrollableView::SetContent(const Ref<ObservableObject>& value, ValueSource source)
	{
		_content->SetContent(value, source);
	}

	const Ref<ObservableObject>& ScrollableView::GetContent() const
	{
		return _content->GetContent();
	}

	void ScrollableView::SetContentTemplate(const Ref<IDataTemplate>& value, ValueSource source)
	{
		_content->SetContentTemplate(value, source);
	}

	const Ref<IDataTemplate>& ScrollableView::GetContentTemplate() const
	{
		return _content->GetContentTemplate();
	}

	void ScrollableView::SetScrollStep(float value, ValueSource source)
	{
		_scrollStep = std::max(1.0f, value);
		SetProperty(ScrollStepProperty, _scrollStep, value, _ScrollStepSource, source);
	}

	void ScrollableView::SetHorizontalOffset(float value)
	{
		value = std::clamp(value, 0.0f, _scrollInfo.MaxHorizontalOffset);

		if(_scrollInfo.HorizontalOffset != value)
		{
			_scrollInfo.HorizontalOffset = value;
			InvalidateArrange();
			ScrollChanged.Invoke(*this);
		}
	}

	void ScrollableView::SetVerticalOffset(float value)
	{
		value = std::clamp(value, 0.0f, _scrollInfo.MaxVerticalOffset);

		if(_scrollInfo.VerticalOffset != value)
		{
			_scrollInfo.VerticalOffset = value;
			InvalidateArrange();
			ScrollChanged.Invoke(*this);
		}
	}

	void ScrollableView::LineUp() 
	{
		SetVerticalOffset(_scrollInfo.VerticalOffset - _scrollStep);
	}

	void ScrollableView::LineDown()
	{
		SetVerticalOffset(_scrollInfo.VerticalOffset + _scrollStep);
	}

	void ScrollableView::LineLeft()
	{
		SetHorizontalOffset(_scrollInfo.HorizontalOffset - _scrollStep);
	}

	void ScrollableView::LineRight()
	{
		SetHorizontalOffset(_scrollInfo.HorizontalOffset + _scrollStep);
	}

	void ScrollableView::ScrollToTop()
	{
		SetVerticalOffset(0);
	}

	void ScrollableView::ScrollToBottom()
	{
		SetVerticalOffset(_scrollInfo.MaxVerticalOffset);
	}

	void ScrollableView::ScrollToLeft()
	{
		SetHorizontalOffset(0);
	}

	void ScrollableView::ScrollToRight()
	{
		SetHorizontalOffset(_scrollInfo.MaxHorizontalOffset);
	}

	void ScrollableView::OnKeyDown(KeyEventArgs& e)
	{
		UIElement::OnKeyDown(e);

		switch(e.Key)
		{
			case KeyCodes::Home:  
				ScrollToTop(); 
				break;

			case KeyCodes::End:
				ScrollToBottom(); 
				break;

			case KeyCodes::Right:
				LineRight();
				break;

			case KeyCodes::Left:
				LineLeft();
				break;

			case KeyCodes::Down:
				LineDown();
				break;

			case KeyCodes::Up:
				LineUp();
				break;

			default:
				break;
		}
	}

	void ScrollableView::OnMouseWheelChanged(MouseWheelEventArgs& e)
	{
		UIElement::OnMouseWheelChanged(e);

		switch(e.Direction)
		{
			case MouseWheelDirection::Normal:
				SetVerticalOffset(_scrollInfo.VerticalOffset - e.ScrolledByY * _scrollStep);
				break;

			case MouseWheelDirection::Flipped:
				SetHorizontalOffset(_scrollInfo.HorizontalOffset - e.ScrolledByX * _scrollStep);
				break;

			default:
				break;
		}

		e.Handled = true;
	}

	FSize ScrollableView::MeasureCore(FSize availableSize)
	{
		FSize desiredSize = UIElement::MeasureCore(availableSize);

		// Max offset calculation
		auto verticalScrollBarWidth = _verticalScrollBar->GetDesiredSize().Width;
		auto horizontalScrollBarHeight = _horizontalScrollBar->GetDesiredSize().Height;

		FSize viewport =
		{
			.Width = std::min(desiredSize.Width, availableSize.Width) - verticalScrollBarWidth,
			.Height = std::min(desiredSize.Height, availableSize.Height) - horizontalScrollBarHeight
		};

		FSize extent = _content->GetDesiredSize();

		auto dx = extent.Width - viewport.Width;
		auto dy = extent.Height - viewport.Height;

		_scrollInfo.Viewport = viewport;
		_scrollInfo.Extent = extent;
		_scrollInfo.MaxHorizontalOffset = dy > 0.0f ? dx : std::max(0.0f, dx - verticalScrollBarWidth);
		_scrollInfo.MaxVerticalOffset = dx > 0.0f ? dy : std::max(0.0f, dy - horizontalScrollBarHeight);

		ScrollMeasured.Invoke(*this);

		return desiredSize;
	}

	FSize ScrollableView::MeasureContent(FSize availableSize)
	{
		for(auto& child : GetChildren())
		{
			child->Measure(availableSize);
		}

		FSize size {};
		size.Width += _verticalScrollBar->GetDesiredSize().Width;
		size.Height += _horizontalScrollBar->GetDesiredSize().Height;
		size += _content->GetDesiredSize();

		return size;
	}

	void ScrollableView::ArrangeContent(FRect rect)
	{
		FRect remainingRect = rect;

		auto verticalScrollBarWidth = _verticalScrollBar->GetDesiredSize().Width;
		auto horizontalScrollBarHeight = _horizontalScrollBar->GetDesiredSize().Height;		

		// Vertical ScrollBar
		float width = _scrollInfo.MaxVerticalOffset > 0.0f ? std::min(verticalScrollBarWidth, remainingRect.w) : 0.0f;
		_verticalScrollBar->Arrange({
			.x = remainingRect.x + remainingRect.w - width,
			.y = remainingRect.y,
			.w = width,
			.h = remainingRect.h
		});

		remainingRect.w -= width;

		// Horizontal ScrollBar
		float height = _scrollInfo.MaxHorizontalOffset > 0.0f ? std::min(horizontalScrollBarHeight, remainingRect.h) : 0.0f;
		_horizontalScrollBar->Arrange({
			.x = remainingRect.x,
			.y = remainingRect.y + remainingRect.h - height,
			.w = remainingRect.w,
			.h = height
		});

		remainingRect.h -= height;

		// Content
		_content->Arrange(remainingRect);
	}

	void ScrollableView::BuildTemplate()
	{
		constexpr float ScrollBarLenght = 15;

		_verticalScrollBar = New<ScrollBar>();
		_verticalScrollBar->SetName("VerticalScrollBar");
		_verticalScrollBar->SetOrientation(Orientation::Vertical);
		_verticalScrollBar->SetWidth(ScrollBarLenght);
		
		_horizontalScrollBar = New<ScrollBar>();
		_horizontalScrollBar->SetName("HorizontalScrollBar");
		_horizontalScrollBar->SetOrientation(Orientation::Horizontal);
		_horizontalScrollBar->SetHeight(ScrollBarLenght);

		_content = New<ScrollContentPresenter>(_scrollInfo);

		AddChild(_verticalScrollBar);
		AddChild(_horizontalScrollBar);
		AddChild(_content);
	}
}

