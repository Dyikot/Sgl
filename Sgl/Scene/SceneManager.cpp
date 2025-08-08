#include "SceneManager.h"

namespace Sgl
{
	SceneManager::~SceneManager()
	{
		Clear();
	}

	void SceneManager::Push(SceneFactory sceneFactory)
	{
		_sceneFactoriesQueue.push(std::move(sceneFactory));
	}

	void SceneManager::Pop() noexcept
	{
		if(_scenesToDestory < _scenes.size())
		{
			_scenesToDestory++;
		}
	}

	void SceneManager::Clear()
	{
		while(!_scenes.empty())
		{
			DestroyScene();
		}
	}

	Scene* SceneManager::GetCurrentScene()
	{
		return _scenes.empty() ? nullptr : _scenes.top().Pointer();
	}

	Scene* SceneManager::GetNextScene()
	{
		while(true)
		{
			if(_scenesToDestory > 0)
			{
				DestroyScene();
			}
			else if(!_sceneFactoriesQueue.empty())
			{
				CreateScene();
			}
			else
			{
				return GetCurrentScene();
			}
		}
	}

	void SceneManager::CreateScene()
	{
		auto scene = _sceneFactoriesQueue.front()();
		scene->OnCreated();

		if(!_scenes.empty())
		{
			_scenes.top()->OnStopped();
		}

		_scenes.push(std::move(scene));
		_sceneFactoriesQueue.pop();
	}

	void SceneManager::DestroyScene()
	{
		_scenesToDestory--;
		_scenes.top()->OnDestroying();
		_scenes.pop();

		if(!_scenes.empty())
		{
			_scenes.top()->OnResumed();
		}
	}
}