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
	private:
		using Clock = std::chrono::high_resolution_clock;
		using TimePoint = std::chrono::steady_clock::time_point;

		bool _isRunning = false;
		TimeSpan _elapsed = TimeSpan::Zero();
		TimePoint _start;
	public:
		/// <summary>
		/// Initializes and starts a new stopwatch instance.
		/// </summary>
		/// <returns>A new Stopwatch instance that has already started.</returns>
		static Stopwatch StartNew();

		/// <summary>
		/// Starts the stopwatch if it is not already running.
		/// </summary>
		void Start();

		/// <summary>
		/// Stops, resets, and restarts the stopwatch.
		/// </summary>
		void Restart();

		/// <summary>
		/// Stops the stopwatch and resets the elapsed time to zero.
		/// </summary>
		void Reset();

		/// <summary>
		/// Pause the stopwatch.
		/// </summary>
		void Pause();

		/// <summary>
		/// Gets a value indicating whether the stopwatch is currently running.
		/// </summary>
		/// <returns>True if the stopwatch is running; otherwise, false.</returns>
		bool IsRunning() const;

		/// <summary>
		/// Gets the total elapsed time measured by the stopwatch.
		/// </summary>
		/// <returns>The TimeSpan representing the total elapsed time.</returns>
		TimeSpan Elapsed();
	private:
		TimeSpan GetElapsedTime();
	};
}