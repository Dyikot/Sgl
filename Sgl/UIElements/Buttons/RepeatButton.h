#pragma once

#include "Button.h"
#include "../../Base/Threading/DispatcherTimer.h"

namespace Sgl::UIElements
{
	class RepeatButton : public Button
	{
	public:
		RepeatButton();
		RepeatButton(RepeatButton&& other) noexcept;

		void SetDelay(uint32_t value, ValueSource source = ValueSource::Local);
		uint32_t GetDelay() const { return _delay; }

		void SetInterval(uint32_t value, ValueSource source = ValueSource::Local);
		uint32_t GetInterval() const { return _interval; }

		static inline StyleableProperty DelayProperty { &SetDelay, &GetDelay };
		static inline StyleableProperty IntervalProperty { &SetInterval, &GetInterval };
	protected:
		void OnMouseDown(MouseClickEventArgs& e) override;
		void OnMouseUp(MouseClickEventArgs& e) override;		
	private:
		void StartTimer();
		void ResetTimer();
	private:
		std::optional<DispatcherTimer> _timer;
		uint32_t _delay = 300;
		uint32_t _interval = 200;

		ValueSource _delaySource {};
		ValueSource _intervalSource {};
	};
}