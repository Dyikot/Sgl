#pragma once

#include <chrono>
#include <future>
#include "../../Events/Event.h"

namespace Sgl
{
	struct TimeElapsedEventArgs: EventArgs
	{
		std::chrono::milliseconds Duration;
	};

	class Timer
	{
	public:
		using TimeElapsedHandler = EventHandler<Timer, TimeElapsedEventArgs>;
		const std::chrono::milliseconds Duration;
	private:
		bool _paused = true;
		bool _elapsed = false;
		std::chrono::milliseconds _timeElapsed;
		std::chrono::steady_clock::time_point _start;
		std::thread _thread;
		std::condition_variable _conditionVariable;
	public:
		Timer(std::chrono::milliseconds duration) noexcept;
		Timer(std::chrono::seconds duration) noexcept;
		Timer(const Timer& timer) = delete;
		Timer(Timer&& timer) = delete;
		~Timer();

		Event<TimeElapsedHandler> Elapsed;

		void Start() noexcept;
		void Restart() noexcept;
		void Pause();
		void Reset() noexcept;
		bool IsPaused() const noexcept { return _paused; }
		bool IsElapsed() const noexcept { return _elapsed; }
	private:
		void Wait();
		void OnTimerElapsed(Timer* sender, const TimeElapsedEventArgs& e);
	};
}