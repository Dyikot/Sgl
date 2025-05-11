#include "Scene.h"

namespace Sgl
{
	Scene::Scene():
		Components(*this)
	{}

	void Scene::OnRender(RenderContext renderContext) const
	{
		renderContext.SetSceneBackground(*this);
		Components.OnRender(renderContext);
	}

	void Scene::OnMouseMove(const MouseButtonEventArgs& e)
	{
		UIElement::OnMouseMove(e);
		Components.OnMouseMove(e);
	}

	void Scene::OnMouseDown(const MouseButtonEventArgs& e)
	{
		UIElement::OnMouseDown(e);
		Components.OnMouseDown(e);
	}

	void Scene::OnMouseUp(const MouseButtonEventArgs& e)
	{
		UIElement::OnMouseUp(e);
		Components.OnMouseUp(e);
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
		while(true)
		{
			if(_popScenes > 0)
			{
				DestroyScene();
				continue;
			}
			else if(!_scenesBuildersQueue.empty())
			{
				BuildScene();
				continue;
			}
			else if(!_scenes.empty())
			{
				return _scenes.top();
			}
			else
			{
				return nullptr;
			}
		}
	}

	void SceneManager::BuildScene() noexcept
	{
		_scenes.push(_scenesBuildersQueue.front()());
		_scenesBuildersQueue.pop();
	}

	void SceneManager::DestroyScene() noexcept
	{
		_popScenes--;
		_scenes.pop();
	}
}