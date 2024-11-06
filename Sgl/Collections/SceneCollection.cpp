#include "SceneCollection.h"

namespace Sgl
{
	void SceneCollection::Add(Scene* scene)
	{
		push(scene);
	}

	void SceneCollection::RemoveCurrent()
	{
		delete top();
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
			RemoveCurrent();
		}
	}
}