#include "RepeatButton.h"

namespace Sgl::UIElements
{
	RepeatButton::RepeatButton()
	{
		Name = "RepeatButton";
	}

	RepeatButton::RepeatButton(RepeatButton&& other) noexcept:
		Button(std::move(other)),
		_delay(other._delay),
		_interval(other._interval)
	{}

	void RepeatButton::SetDelay(uint32_t value, ValueSource source)
	{
		if(SetProperty(DelayProperty, _delay, value, _delaySource, source))
		{
			ResetTimer();
		}
	}

	void RepeatButton::SetInterval(uint32_t value, ValueSource source)
	{
		if(SetProperty(IntervalProperty, _interval, value, _intervalSource, source))
		{
			ResetTimer();
		}
	}

	void RepeatButton::OnMouseDown(MouseClickEventArgs& e)
	{
		Button::OnMouseDown(e);

		if(e.Button == MouseButton::Left)
		{
			StartTimer();
		}
	}

	void RepeatButton::OnMouseUp(MouseClickEventArgs& e)
	{
		Button::OnMouseUp(e);

		if(e.Button == MouseButton::Left)
		{
			ResetTimer();
		}
	}

	void RepeatButton::StartTimer()
	{
		_timer.emplace(_interval, _delay);
		_timer->Tick += [this](DispatcherTimer& sender, EventArgs e)
		{
			OnClick();
		};

		_timer->Start();
	}

	void RepeatButton::ResetTimer()
	{
		if(_timer)
		{
			_timer.reset();
		}
	}
}

