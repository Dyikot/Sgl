#include "Animation.h"

namespace Sgl
{
	Animation::Animation(const Timer& timer, size_t actions) noexcept:
		_timer(timer),
		Actions(actions)
	{
	
	}

	void Animation::Handle()
	{
		if(_timer.Elapsed())
		{
			OnElapsed(
				{ 
					.Period = _timer.Duration,
					.ActionsCompleted = _actionsCompleted
				});
			_actionsCompleted++;

			if(_actionsCompleted == Actions)
			{
				OnAnimationCompleted(
					{
						.Period = _timer.Duration,
						.ActionsCompleted = _actionsCompleted
					});
				Stop();
				return;
			}

			_timer.Start();
		}	
	}

	void Animation::Start()
	{
		OnAnimationStarted(
			{
				.Period = _timer.Duration,
				.ActionsCompleted = _actionsCompleted
			});
		_timer.Start();
	}


	void Animation::Stop()
	{
		_actionsCompleted = 0;
		_timer.Reset();
	}

	void Animation::OnAnimationStarted(const AnimationEventArgs& e)
	{
		if(AnimationStarted)
		{
			AnimationStarted(this, e);
		}
	}

	void Animation::OnAnimationCompleted(const AnimationEventArgs& e)
	{
		if(AnimationCompleted)
		{
			AnimationCompleted(this, e);
		}
	}

	void Animation::OnElapsed(const AnimationEventArgs& e)
	{
		if(Elapsed)
		{
			Elapsed(this, e);
		}
	}
}