#include "ScrollBar.h"
#include "../Buttons/RepeatButton.h"
#include "../../Layout/LayoutHelper.h"
#include "../../Base/Logging.h"

namespace Sgl::UIElements
{
	namespace
	{
		class ThumbStyle : public UIElementStyle
		{
		public:
			void OnBase(StyleContext& context) override
			{
				context.Set(Button::BackgroundProperty, Colors::Grey_3);
			}
		};

		class TrackStyle : public ButtonStyle
		{
		public:
			void OnBase(StyleContext& context) override
			{
				context.Set(Button::BackgroundProperty, Colors::Grey_1);
				context.Set(Button::CursorProperty, Cursors::Arrow);
			}
		protected:
			void OnHover(StyleContext& context) override {}
			void OnPressed(StyleContext& context) override {}
			void OnFocus(StyleContext& context) override {}
		};

		class Track : public RepeatButton
		{
		public:
			Track()
			{
				SetName("Track");
				SetDelay(150);
				SetInterval(25);
			}
		protected:
			Ref<Style> CreateDefaultStyle() const override
			{
				return New<TrackStyle>();
			}

			void ArrangeContent(FRect rect) override
			{
				auto parent = static_cast<ScrollBar*>(GetParent());
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
	}

	ScrollBar::ScrollBar()
	{
		SetName("ScrollBar");
		BuildTemplate();
	}

	void ScrollBar::SetViewportSize(float value)
	{
		value = std::clamp(value, 0.0f, 1.0f);
		SetProperty(ViewportSizeProperty, _viewportSize, value, _viewportSizeSource);
	}

	void ScrollBar::SetSmallChange(float value)
	{
		SetProperty(SmallChangeProperty, _smallChange, Clamp(value), _smallChangeSource);
	}

	void ScrollBar::SetLargeChange(float value)
	{
		SetProperty(LargeChangeProperty, _largeChange, Clamp(value), _largeChangeSource);
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

	void ScrollBar::OnPropertyChanged(PropertyBase& property)
	{
		RangeBase::OnPropertyChanged(property);

		if(property == ViewportSizeProperty)
		{
			_scrollScale = _viewportSize < 1.0f ? 1.0f / (1.0f - _viewportSize) : 0.0f;
			InvalidateArrange();
		}
	}

	void ScrollBar::OnValueChanged(float value)
	{
		RangeBase::OnValueChanged(value);
		InvalidateArrange();
	}

	Ref<Style> ScrollBar::CreateDefaultStyle() const
	{
		return New<ScrollBarStyle>();
	}

	void ScrollBar::BuildTemplate()
	{
		_thumb = New<UIElement>();
		_thumb->SetName("Thumb");
		_thumb->SetStyle(New<ThumbStyle>());
		
		_track = New<Track>();
		_track->SetContent(_thumb);
		_track->MouseDown += [this](UIElement& sender, MouseClickEventArgs& e)
		{
			_dragValue = GetValue();
			_clickValue = ValueAtPosition(e.X, e.Y);
			_wasHoverThumb = _thumb->IsMouseOver();
		};
		_track->Click += [this](Button& sender, EventArgs e)
		{
			float x = 0, y = 0;
			SDL_GetMouseState(&x, &y);

			auto newValue = ValueAtPosition(x, y);

			if(_thumb->IsMousePressed())
			{
				SetValue(_dragValue + (newValue - _clickValue) * _scrollScale);
			}
			else if(!_wasHoverThumb && !IsPointInRect(x, y, _thumb->GetBounds()))
			{
				auto oldValue = GetValue();
				auto pageSize = GetPageSize();
				auto sign = (newValue > oldValue) - (oldValue > newValue);
				SetValue(oldValue + sign * pageSize);
			}

			//Logging::LogInfo("Value: {}", GetValue());
		};

		AddChild(_track);
	}

	float ScrollBar::GetPageSize() const
	{
		return _viewportSize * _scrollScale * (GetMaxValue() - GetMinValue());
	}

	void ScrollBarStyle::OnBase(StyleContext& context)
	{
		context.Set(ScrollBar::WidthProperty, 15);
		context.Set(ScrollBar::HeightProperty, 15);
	}
}

