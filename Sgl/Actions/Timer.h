#pragma once

#include <chrono>
#include <functional>
#include <future>

namespace Sgl
{
	class Timer
	{
	public:
		const std::chrono::milliseconds Duration;
		const bool SeparateThread;
	private:
		bool _isPaused = true;
		bool _isElapsed = false;
		std::chrono::milliseconds _timeElapsed;
		std::chrono::steady_clock::time_point _start;
	public:
		Timer(std::chrono::milliseconds duration) noexcept;
		Timer(const Timer& timer) noexcept;
		~Timer() noexcept;

		void Start() noexcept;
		void Pause() noexcept;
		void Reset() noexcept;
		bool IsPaused() const noexcept
		{
			return _isPaused;
		}
		bool Elapsed() const noexcept;
	private:
		void WaitOnSeparateThread();
	};
}