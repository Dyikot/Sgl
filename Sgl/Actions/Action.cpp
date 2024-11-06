#include "Action.h"

namespace Sgl
{
	Action::Action(const Timer& timer) noexcept:
		_timer(timer)
	{
		
	}

	void Action::Handle()
	{
		if(_timer.Elapsed())
		{
			OnElapsed({ .Period = _timer.Duration });
			_timer.Start();
		}			
	}

	void Action::OnElapsed(const ActionEventArgs& e)
	{
		if(Elapsed)
		{
			Elapsed(this, e);
		}
	}
}