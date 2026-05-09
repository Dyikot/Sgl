#pragma once

#include "Button.h"
#include "../../Base/Time/DispatcherTimer.h"

namespace Sgl::UIElements
{
	class RepearButton : public Button
	{
	public:
		RepearButton() = default;
		RepearButton(RepearButton&& other) noexcept;

		void SetDelay(uint32_t value, ValueSource source = ValueSource::Local);
		uint32_t GetDelay() const { return _delay; }

		void SetInterval(uint32_t value, ValueSource source = ValueSource::Local);
		uint32_t GetInterval() const { return _interval; }

		static inline StyleableProperty DelayProperty { &SetDelay, &GetDelay };
		static inline StyleableProperty IntervalProperty { &SetInterval, &GetInterval };
	protected:
		void OnMouseDown(MouseButtonEventArgs e) override;
		void OnMouseUp(MouseButtonEventArgs e) override;		
	private:
		void StartTimer();
		void ResetTimer();
	private:
		std::optional<DispatcherTimer> _timer;
		uint32_t _delay = 250;
		uint32_t _interval = 200;

		ValueSource _delaySource {};
		ValueSource _intervalSource {};
	};
}