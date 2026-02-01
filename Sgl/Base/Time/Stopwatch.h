#pragma once

#include "TimeSpan.h"
#include <chrono>

namespace Sgl
{
	/// <summary>
	/// Provides functionality to measure elapsed time with high precision.
	/// </summary>
	class Stopwatch
	{
	public:
		/// <summary>
		/// Initializes and starts a new stopwatch instance.
		/// </summary>
		/// <returns>A new Stopwatch instance that has already started.</returns>
		static Stopwatch StartNew();

		/// <summary>
		/// Starts the stopwatch if it is not already running.
		/// </summary>
		void Start() noexcept;

		/// <summary>
		/// Resets, and restarts the stopwatch.
		/// </summary>
		void Restart() noexcept;

		/// <summary>
		/// Stops the stopwatch and resets the elapsed time to zero.
		/// </summary>
		void Reset() noexcept;

		/// <summary>
		/// Pause the stopwatch.
		/// </summary>
		void Pause() noexcept;

		/// <summary>
		/// Gets a value indicating whether the stopwatch is currently running.
		/// </summary>
		/// <returns>True if the stopwatch is running; otherwise, false.</returns>
		bool IsRunning() const noexcept;

		/// <summary>
		/// Gets the total elapsed time measured by the stopwatch.
		/// </summary>
		/// <returns>The TimeSpan representing the total elapsed time.</returns>
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