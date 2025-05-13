#include "Scene.h"

namespace Sgl
{
	Scene::Scene():
		Style(Properties)
	{}

	void Scene::OnRender(RenderContext renderContext) const
	{
		renderContext.SetSceneBackground(*this);
		UIElements.OnRender(renderContext);
	}

	void SceneManager::Push(SceneFactory sceneFactory)
	{
		_sceneFactoriesQueue.push(std::move(sceneFactory));
	}

	void SceneManager::Pop() noexcept
	{
		if(_popScenes < _scenes.size())
		{
			_popScenes++;
		}
	}

	std::shared_ptr<Scene> SceneManager::GetCurrentScene()
	{
		return _scenes.empty() ? nullptr : _scenes.top();
	}

	std::shared_ptr<Scene> SceneManager::GetNextScene()
	{
		while(true)
		{
			if(_popScenes > 0)
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

	void SceneManager::CreateScene() noexcept
	{
		_scenes.push(_sceneFactoriesQueue.front()());
		_sceneFactoriesQueue.pop();
	}

	void SceneManager::DestroyScene() noexcept
	{
		_popScenes--;
		_scenes.pop();
	}
}