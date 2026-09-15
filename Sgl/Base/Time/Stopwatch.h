#pragma once

#include "TimeSpan.h"
#include <chrono>

namespace Sgl
{
	//! @brief Provides functionality to measure elapsed time with high precision
	class Stopwatch
	{
	public:
		//! @brief Initializes and starts a new stopwatch instance
		//! @return A new Stopwatch instance that has already started
		static Stopwatch StartNew();

		//! @brief Starts the stopwatch if it is not already running
		void Start() noexcept;

		//! @brief Resets and restarts the stopwatch
		void Restart() noexcept;

		//! @brief Stops the stopwatch and resets the elapsed time to zero
		void Reset() noexcept;

		//! @brief Pauses the stopwatch
		void Pause() noexcept;

		//! @brief Gets a value indicating whether the stopwatch is currently running
		//! @return True if the stopwatch is running; otherwise, false
		bool IsRunning() const noexcept;

		//! @brief Gets the total elapsed time measured by the stopwatch
		//! @return The TimeSpan representing the total elapsed time
		TimeSpan Elapsed() const noexcept;

	private:
		TimeSpan GetElapsedTime() const noexcept;

		using Clock = std::chrono::high_resolution_clock;
		using TimePoint = std::chrono::steady_clock::time_point;

		TimePoint _start;
		TimeSpan _elapsed = TimeSpan::Zero;
		bool _isRunning = false;
	};
}