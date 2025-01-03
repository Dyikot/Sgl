#include "SceneStack.h"

namespace Sgl
{
	SceneStack::~SceneStack()
	{
		Clear();
	}

	void SceneStack::Push(Scene* scene)
	{
		scene->OnLoaded(EventArgs());
		emplace(scene);
	}

	void SceneStack::Pop()
	{
		top()->OnUnloaded(EventArgs());
		pop();
	}

	bool SceneStack::IsCurrentClosed()
	{
		if(top()->IsClosed())
		{
			Pop();
			return true;
		}

		return false;
	}

	void SceneStack::Clear()
	{
		while(!empty())
		{
			Pop();
		}
	}
	size_t SceneStack::Count() const
	{
		return size();
	}
}