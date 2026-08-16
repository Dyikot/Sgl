#include "ScrollBar.h"
#include "../Buttons/RepeatButton.h"
#include "../../Layout/LayoutHelper.h"

namespace Sgl::UIElements
{
	static constexpr Color TrackBackground { 0xf5f5f5ff };
	static constexpr Color ThumbBackground { 0xc2c2c2ff };

	class ScrollBar::Track : public RepeatButton
	{
	public:
		Track()
		{
			Name = "Track";
			SetCursor(Cursors::Arrow);
			SetDelay(150, ValueSource::Default);
			SetInterval(25, ValueSource::Default);
			SetBackground(TrackBackground, ValueSource::Default);
		}
	protected:
		void ArrangeContent(FRect rect) final
		{
			auto parent = static_cast<ScrollBar*>(GetStylingParent());
			auto value = parent->GetRelativeValue();
			auto viewportSize = parent->GetViewportSize();
			auto orientation = parent->GetOrientation();

			auto [width, height] = GetDesiredSize();

			FRect thumbRect = rect;

			if(orientation == Orientation::Horizontal)
			{
				if(width == 0.0f)
				{
					width = rect.w;
				}

				thumbRect.w = width * viewportSize;
				thumbRect.x = rect.x + (rect.w - thumbRect.w) * value;
			}
			else
			{
				if(height == 0.0f)
				{
					height = rect.h;
				}

				thumbRect.h = height * viewportSize;
				thumbRect.y = rect.y + (rect.h - thumbRect.h) * value;
			}

			RepeatButton::ArrangeContent(thumbRect);
		}
	};

	ScrollBar::ScrollBar()
	{
		Name = "ScrollBar";
		BuildTemplate();
	}

	ScrollBar::ScrollBar(ScrollBar&& other) noexcept:
		RangeBase(std::move(other)),
		_thumb(std::move(other._thumb)),
		_track(std::move(other._track)),
		_viewportSize(other._viewportSize)
	{}

	void ScrollBar::SetViewportSize(float value, ValueSource source)
	{
		value = std::clamp(value, 0.0f, 1.0f);

		if(SetProperty(ViewportSizeProperty, _viewportSize, value, _viewportSizeSource, source))
		{
			_scrollScale = 1.0f / (1.0f - _viewportSize);
			InvalidateArrange();
		}
	}

	void ScrollBar::SetSmallChange(float value, ValueSource source)
	{
		value = Clamp(value);
		SetProperty(SmallChangeProperty, _smallChange, value, _smallChangeSource, source);
	}

	void ScrollBar::SetLargeChange(float value, ValueSource source)
	{
		value = Clamp(value);
		SetProperty(LargeChangeProperty, _largeChange, value, _largeChangeSource, source);
	}

	void ScrollBar::LineUp()
	{
		SetValue(GetValue() - _smallChange);
	}

	void ScrollBar::LineDown()
	{
		SetValue(GetValue() + _smallChange);
	}

	void ScrollBar::PageUp()
	{
		SetValue(GetValue() - _largeChange);
	}

	void ScrollBar::PageDown()
	{
		SetValue(GetValue() + _largeChange);
	}

	void ScrollBar::ScrollToHome()
	{
		SetValue(GetMinValue());
	}

	void ScrollBar::ScrollToEnd()
	{
		SetValue(GetMaxValue());
	}

	void ScrollBar::OnValueChanged(float value)
	{
		RangeBase::OnValueChanged(value);
		InvalidateArrange();
	}

	void ScrollBar::OnMouseDown(MouseClickEventArgs& e)
	{
		RangeBase::OnMouseDown(e);

		_dragValue = GetValue();
		_clickValue = ValueAtPosition(e.X, e.Y);
		_wasHoverThumb = _thumb->IsMouseOver();
	}

	void ScrollBar::BuildTemplate()
	{
		_thumb = New<UIElement>();
		_thumb->Name = "Thumb";
		_thumb->SetBackground(ThumbBackground, ValueSource::Default);

		_track = New<Track>();
		_track->SetContent(_thumb);
		_track->Click += [this](Button& sender, MouseClickEventArgs& e)
		{
			auto newValue = ValueAtPosition(e.X, e.Y);

			if(_wasHoverThumb && e.ClicksNumber == 0) // Dragging
			{
				SetValue(_dragValue + (newValue - _clickValue) * _scrollScale);
			}
			else if(!IsPointInRect(e.X, e.Y, _thumb->GetBounds()))
			{
				auto oldValue = GetValue();
				auto pageSize = GetPageSize();
				auto sign = (newValue > oldValue) - (oldValue > newValue);
				SetValue(oldValue + sign * pageSize);
			}
		};

		SetTemplate(_track);
	}

	float ScrollBar::GetPageSize() const
	{
		return _viewportSize * _scrollScale * (GetMaxValue() - GetMinValue());
	}
}

