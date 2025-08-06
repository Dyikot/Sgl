#pragma once

#include "Time/Stopwatch.h"

namespace Sgl
{
	class FPSCounter
	{
	private:
		static constexpr TimeSpan Period = TimeSpan::FromSeconds(1);

		Stopwatch _stopwatch;
		size_t _fps = 0;
		size_t _frames = 0;
		TimeSpan _elapsed = TimeSpan::Zero;
	public:
		size_t GetFps() const { return _fps; }

		void OnFrameStart()
		{
			_elapsed += _stopwatch.Elapsed();
			_frames++;

			if(_elapsed >= Period)
			{
				_fps = _frames;

				_frames = 0;
				_elapsed = TimeSpan::Zero;
			}

			_stopwatch.Restart();
		}
	};
}