#pragma once

#include "../Event.h"
#include "../Time/TimeSpan.h"

namespace Sgl
{
	//! @brief Represents a timer that raises the Tick event at regular intervals
	//! on the UI thread, suitable for UI-related timed operations. Must be created on the UI thread.
	class DispatcherTimer
	{
	public:
		using TimeElapsedEventHandler = EventHandler<DispatcherTimer>;
	public:
		explicit DispatcherTimer(TimeSpan interval, TimeSpan delay = TimeSpan::Zero);
		explicit DispatcherTimer(int64_t intervalMilliseconds, int64_t delayMilliseconds = 0);
		DispatcherTimer(const DispatcherTimer&) = delete;
		DispatcherTimer(DispatcherTimer&&) = delete;
		~DispatcherTimer();

		//! @brief Gets the initial delay before the timer starts its first interval
		const TimeSpan Delay;

		//! @brief Gets the interval for which the timer is set
		const TimeSpan Interval;

		//! @brief Occurs when the timer interval has elapsed
		Event<TimeElapsedEventHandler> Tick;

		//! @brief Starts the timer
		void Start();
		
		//! @brief Stop the timer
		void Stop();
	private:
		bool _running = false;
	};
}