#pragma once

#include "../Events/Event.h"
#include "Timer.h"
#include "IAction.h"

namespace Sgl
{
	struct ActionEventArgs: EventArgs
	{
		std::chrono::milliseconds Period;
	};

	class Action: public IAction
	{
	public:
		using ActionEventHandler = std::function<void(Action*, const ActionEventArgs&)>;
	private:
		Timer _timer;
	public:
		Action(const Timer& timer) noexcept;

		Event<ActionEventHandler> Elapsed;

		void Handle() override;
		void Start() override 
		{
			_timer.Start(); 
		}
		void Stop() override 
		{
			_timer.Reset();
		}
		void Pause() override 
		{ 
			_timer.Pause(); 
		}
	protected:
		virtual void OnElapsed(const ActionEventArgs& e);
	};
}