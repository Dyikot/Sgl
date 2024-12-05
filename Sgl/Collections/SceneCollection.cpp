#include "SceneCollection.h"
#include "../UI/Scene.h"

namespace Sgl
{
	void SceneCollection::Push(Scene* scene)
	{
		scene->OnLoaded(EventArgs());
		push(scene);
	}

	void SceneCollection::Pop()
	{
		auto scene = top();
		scene->OnUnloaded(EventArgs());
		delete scene;
		pop();
	}

	bool SceneCollection::Empty() const
	{
		return empty();
	}

	Scene* const SceneCollection::Current() const
	{
		return top();
	}

	void SceneCollection::Clear()
	{
		while(!empty())
		{
			Pop();
		}
	}
}