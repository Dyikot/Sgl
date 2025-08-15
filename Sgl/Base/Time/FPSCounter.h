#pragma once

#include "Stopwatch.h"

namespace Sgl
{
	class FPSCounter
	{
	private:
		static constexpr TimeSpan Period = TimeSpan::FromSeconds(1);

		size_t _fps = 0;
		size_t _frames = 0;
		TimeSpan _elapsed = TimeSpan::Zero;
	public:
		size_t GetFps() const { return _fps; }

		void Update(TimeSpan elapsed)
		{
			_frames++;
			_elapsed += elapsed;

			if(_elapsed >= Period)
			{
				_fps = _frames;

				_frames = 0;
				_elapsed = TimeSpan::Zero;
			}
		}
	};
}