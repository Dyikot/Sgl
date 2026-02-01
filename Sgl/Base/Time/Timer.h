#pragma once

#include <thread>
#include "../Event.h"
#include "Stopwatch.h"

namespace Sgl
{
	/// <summary>
	/// Represents a timer that raises an event after a specified duration.
	/// </summary>
	class Timer final
	{
	public:
		using TimeElapsedEventHandler = EventHandler<Timer>;
	public:
		explicit Timer(TimeSpan interval) noexcept;
		explicit Timer(int64_t milliseconds) noexcept;

		Timer(const Timer&) = delete;
		Timer(Timer&&) = default;

		/// <summary>
		/// Gets the duration for which the timer is set.
		/// </summary>
		const TimeSpan Duration;

		/// <summary>
		/// Occurs when the timer reaches its specified duration.
		/// </summary>
		Event<TimeElapsedEventHandler> Elapsed;

		/// <summary>
		/// Gets or sets a value indicating whether the timer should automatically restart its countdown after elapsing.
		/// </summary>
		bool AutoRestart = false;

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
		void Pause() noexcept;

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
		Stopwatch _stopwatch;
		std::jthread _thread;
	};
}