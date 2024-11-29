#include "SceneStack.h"
#include "../UI/Scene.h"

namespace Sgl
{
	void SceneStack::Push(Scene* scene)
	{
		scene->OnLoaded(EventArgs{});
		push(scene);
	}

	void SceneStack::Pop()
	{
		auto scene = top();
		scene->OnUnloaded(EventArgs{});
		delete scene;
		pop();
	}

	bool SceneStack::Empty() const
	{
		return empty();
	}

	Scene* const SceneStack::Current() const
	{
		return top();
	}

	void SceneStack::Clear()
	{
		while(!empty())
		{
			Pop();
		}
	}
}