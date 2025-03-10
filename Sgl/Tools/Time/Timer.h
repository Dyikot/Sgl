#pragma once

#include <chrono>
#include <future>
#include "../../Events/Event.h"
#include "TimeSpan.h"

namespace Sgl
{
	struct TimeElapsedEventArgs: EventArgs
	{
		TimeSpan Duration;
	};

	class Timer
	{
	public:
		using TimeElapsedHandler = EventHandler<Timer, TimeElapsedEventArgs>;
	private:
		bool _paused = true;
		bool _elapsed = false;
		std::chrono::milliseconds _timeElapsed;
		std::chrono::milliseconds _duration;
		std::thread _thread;
		std::condition_variable _conditionVariable;
		std::chrono::steady_clock::time_point _start;
	public:
		Timer(TimeSpan timespan) noexcept;
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
	};
}