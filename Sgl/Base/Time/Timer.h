#pragma once

#include <thread>
#include "../Event.h"
#include "Stopwatch.h"

namespace Sgl
{
	//! @brief Represents a timer that raises an event after a specified duration
	class Timer
	{
	public:
		using TimeElapsedEventHandler = EventHandler<Timer>;
	public:
		//! @brief Initializes a new timer with the specified interval
		//! @param interval The duration of the timer
		explicit Timer(TimeSpan interval) noexcept;

		//! @brief Initializes a new timer with the specified interval in milliseconds
		//! @param milliseconds The duration of the timer in milliseconds
		explicit Timer(int64_t milliseconds) noexcept;

		Timer(const Timer&) = delete;
		Timer(Timer&&) = default;

		//! @brief Gets the duration for which the timer is set
		const TimeSpan Duration;

		//! @brief Occurs when the timer reaches its specified duration
		Event<TimeElapsedEventHandler> Elapsed;

		//! @brief Gets or sets a value indicating whether the timer should automatically restart its countdown after elapsing
		bool AutoRestart = false;

		//! @brief Starts the timer
		void Start();

		//! @brief Restarts the timer by resetting and starting it again
		void Restart() noexcept;

		//! @brief Pauses the timer if it is running
		void Pause() noexcept;

		//! @brief Resets the timer to its initial state
		void Reset() noexcept;

		//! @brief Gets a value indicating whether the timer is paused
		//! @return True if the timer is paused; otherwise, false
		bool IsPaused() const noexcept;

		//! @brief Gets a value indicating whether the timer has completed its duration
		//! @return True if the timer has elapsed; otherwise, false
		bool IsElapsed() const noexcept;

	private:
		Stopwatch _stopwatch;
		std::jthread _thread;
	};
}