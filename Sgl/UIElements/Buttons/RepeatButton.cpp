#include "RepeatButton.h"
#include "../../Base/Time/DispatcherTimer.h"

namespace Sgl::UIElements
{
	RepearButton::RepearButton(RepearButton&& other) noexcept:
		Button(std::move(other)),
		_timer(std::move(other._timer)),
		_delay(other._delay),
		_interval(other._interval)
	{}

	void RepearButton::SetDelay(uint32_t value, ValueSource source)
	{
		if(SetProperty(DelayProperty, _delay, value, _delaySource, source))
		{
			ResetTimer();
		}
	}

	void RepearButton::SetInterval(uint32_t value, ValueSource source)
	{
		if(SetProperty(IntervalProperty, _interval, value, _intervalSource, source))
		{
			ResetTimer();
		}
	}

	void RepearButton::OnMouseDown(MouseButtonEventArgs e)
	{
		ContentUIElement::OnMouseDown(e);

		if(e.Button == MouseButton::Left)
		{
			StartTimer();
		}
	}

	void RepearButton::OnMouseUp(MouseButtonEventArgs e)
	{
		ContentUIElement::OnMouseUp(e);

		if(e.Button == MouseButton::Left)
		{
			ResetTimer();
		}
	}

	void RepearButton::StartTimer()
	{
		_timer.emplace(_interval);
		_timer->Tick += [this](DispatcherTimer& sender, EventArgs e)
		{
			OnClick();
		};

		_timer->Start();
	}

	void RepearButton::ResetTimer()
	{
		if(_timer)
		{
			_timer.reset();
		}	
	}
}

