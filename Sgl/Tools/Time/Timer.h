#pragma once

#include <future>
#include "../../Events/Event.h"
#include "Stopwatch.h"

namespace Sgl
{
	class Timer;
	using TimeElapsedHandler = EventHandler<Timer, EventArgs>;

	class Timer
	{
	public:
		const TimeSpan Duration;
	private:
		bool _isPaused = true;
		bool _isElapsed = false;
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
		bool IsPaused() const noexcept;
		bool IsElapsed() const noexcept;
	private:
		void Wait();
	};
}