#include "Scene.h"

namespace Sgl
{
	Scene::Scene():
		ClassStyle(_properties),
		UIElements(*this)
	{}

	void Scene::OnRender(RenderContext renderContext) const
	{
		if(_properties.BackgroundTexture)
		{
			renderContext.FillBackground(*_properties.BackgroundTexture, _properties.BackgroundColor);
		}
		else
		{
			renderContext.FillBackground(_properties.BackgroundColor);
		}

		for(IUIElement& element : UIElements)
		{
			element.OnRender(renderContext);
		}
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