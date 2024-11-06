#pragma once

#include "../Events/Event.h"
#include "Timer.h"
#include "IAction.h"

namespace Sgl
{
	struct AnimationEventArgs: EventArgs
	{
		std::chrono::milliseconds Period;
		size_t ActionsCompleted;
	};

	class Animation: public IAction
	{
	public:
		using AnimationEventHandler = std::function<void(Animation*, const AnimationEventArgs&)>;
		const size_t Actions;
	protected:
		size_t _actionsCompleted = 0;
	private:
		Timer _timer;
	public:
		Animation(const Timer& timer, size_t actions) noexcept;
		virtual ~Animation() = default;

		Event<AnimationEventHandler> AnimationCompleted;
		Event<AnimationEventHandler> AnimationStarted;
		Event<AnimationEventHandler> Elapsed;

		void Handle() override;
		void Start() override;
		void Stop() override;
		void Pause() override
		{ 
			_timer.Pause();
		}
	protected:
		virtual void OnAnimationStarted(const AnimationEventArgs& e);
		virtual void OnAnimationCompleted(const AnimationEventArgs& e);
		virtual void OnElapsed(const AnimationEventArgs& e);
	};
}