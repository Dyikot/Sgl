#pragma once

#include "../Event.h"
#include "../Time/TimeSpan.h"

namespace Sgl
{
	/// <summary>
	/// Represents a timer that raises the Tick event at regular intervals 
	/// on the UI thread, suitable for UI-related timed operations. Must be created on the UI thread.
	/// </summary>
	class DispatcherTimer final
	{
	public:
		using TimeElapsedEventHandler = EventHandler<DispatcherTimer>;
	public:
		explicit DispatcherTimer(TimeSpan interval);
		explicit DispatcherTimer(int64_t milliseconds);
		DispatcherTimer(const DispatcherTimer&) = delete;
		DispatcherTimer(DispatcherTimer&&) = default;
		~DispatcherTimer();

		/// <summary>
		/// Gets the interval for which the timer is set.
		/// </summary>
		const TimeSpan Interval;

		/// <summary>
		/// Occurs when the timer interval has elapsed.
		/// </summary>
		Event<TimeElapsedEventHandler> Tick;

		/// <summary>
		/// Starts the timer.
		/// </summary>
		void Start();

		/// <summary>
		/// Stop the timer.
		/// </summary>
		void Stop();
	private:
		bool _running = false;
	};
}