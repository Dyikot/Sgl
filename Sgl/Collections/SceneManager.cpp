#include "SceneManager.h"
#include "../UI/Scene.h"

namespace Sgl
{
	SceneManager::~SceneManager()
	{
		UnloadAll();
	}

	void SceneManager::Load(Scene* scene)
	{
		scene->OnLoaded(EventArgs());
		push(scene);
	}

	void SceneManager::UnloadActive()
	{
		auto scene = top();
		scene->OnUnloaded(EventArgs());
		delete scene;
		pop();
	}

	void SceneManager::UnloadAll()
	{
		while(!empty())
		{
			UnloadActive();
		}
	}
}