#pragma once

#include <future>
#include "../Events/Event.h"
#include "Stopwatch.h"

namespace Sgl
{
	/// <summary>
	/// Represents a timer that raises an event after a specified duration.
	/// </summary>
	class Timer
	{
	public:
		using TimeElapsedHandler = EventHandler<Timer, EventArgs>;

		/// <summary>
		/// Gets the duration for which the timer is set.
		/// </summary>
		const TimeSpan Duration;

		/// <summary>
		/// Occurs when the timer reaches its specified duration.
		/// </summary>
		Event<TimeElapsedHandler> Elapsed;
	private:
		bool _isPaused = true;
		bool _isElapsed = false;
		Stopwatch _stopwatch;
		std::thread _thread;
		std::condition_variable _conditionVariable;
	public:
		/// <summary>
		/// Initializes a new instance of the Timer class with the specified duration.
		/// </summary>
		/// <param name="timespan"> - The time span representing the timer duration.</param>
		Timer(TimeSpan timespan) noexcept;

		Timer(const Timer& timer) = delete;
		Timer(Timer&& timer) = delete;
		~Timer();

		/// <summary>
		/// Starts the timer.
		/// </summary>
		void Start() noexcept;

		/// <summary>
		/// Restarts the timer by resetting and starting it again.
		/// </summary>
		void Restart() noexcept;

		/// <summary>
		/// Pauses the timer if it is running.
		/// </summary>
		void Pause();

		/// <summary>
		/// Resets the timer to its initial state.
		/// </summary>
		void Reset() noexcept;

		/// <summary>
		/// Gets a value indicating whether the timer is paused.
		/// </summary>
		/// <returns>True if the timer is paused; otherwise, false.</returns>
		bool IsPaused() const noexcept;

		/// <summary>
		/// Gets a value indicating whether the timer has completed its duration.
		/// </summary>
		/// <returns>True if the timer has elapsed; otherwise, false.</returns>
		bool IsElapsed() const noexcept;
	private:
		void Wait();
	};
}