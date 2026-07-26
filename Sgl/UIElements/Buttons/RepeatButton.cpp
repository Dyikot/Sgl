#include "RepeatButton.h"
#include "../../Base/Time/DispatcherTimer.h"

namespace Sgl::UIElements
{
	RepeatButton::RepeatButton(RepeatButton&& other) noexcept:
		Button(std::move(other)),
		_timer(std::move(other._timer)),
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

	void RepeatButton::OnMouseDown(MouseButtonEventArgs e)
	{
		Button::OnMouseDown(e);

		if(e.Button == MouseButton::Left)
		{
			StartTimer();
		}
	}

	void RepeatButton::OnMouseUp(MouseButtonEventArgs e)
	{
		Button::OnMouseUp(e);

		if(e.Button == MouseButton::Left)
		{
			ResetTimer();
		}
	}

	void RepeatButton::StartTimer()
	{
		_timer.emplace(_interval);
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

