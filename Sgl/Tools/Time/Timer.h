#pragma once

#include <future>
#include "../../Events/Event.h"
#include "Stopwatch.h"

namespace Sgl
{
	struct TimeElapsedEventArgs: EventArgs
	{
		TimeSpan duration;
	};

	class Timer;
	using TimeElapsedHandler = EventHandler<Timer, TimeElapsedEventArgs>;

	class Timer
	{
	public:
		const TimeSpan duration;
	private:
		bool _paused = true;
		bool _elapsed = false;
		Stopwatch _stopwatch;
		std::thread _thread;
		std::condition_variable _conditionVariable;
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