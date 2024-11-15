#include "SceneCollection.h"

namespace Sgl
{
	void SceneCollection::Push(Scene* scene)
	{
		push(scene);
	}

	void SceneCollection::Pop()
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
			Pop();
		}
	}
}