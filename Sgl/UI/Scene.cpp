#include "Scene.h"

namespace Sgl
{
	void Scene::OnRender(RenderContext renderContext) const
	{
		renderContext.SetSceneBackground(*this);
		components.OnRender(renderContext);
	}

	void Scene::OnMouseMove(const MouseButtonEventArgs& e)
	{
		UIElement::OnMouseMove(e);
		components.OnMouseMove(e);
	}

	void Scene::OnMouseDown(const MouseButtonEventArgs& e)
	{
		UIElement::OnMouseDown(e);
		components.OnMouseDown(e);
	}

	void Scene::OnMouseUp(const MouseButtonEventArgs& e)
	{
		UIElement::OnMouseUp(e);
		components.OnMouseUp(e);
	}

	SceneManager::~SceneManager()
	{
		while(!_scenes.empty())
		{
			UnloadScene();
		}
	}

	void SceneManager::Unload()
	{
		if(_scenesToUnload < _scenes.size())
		{
			_scenesToUnload++;
		}
	}

	SceneState SceneManager::UpdateState()
	{
		if(_scenesToUnload > 0)
		{
			UnloadScene();
			return SceneState::Loading;
		}
		else if(!_scenesBuildersQueue.empty())
		{
			LoadScene();
			return SceneState::Loading;
		}
		else if(!_scenes.empty())
		{
			return SceneState::Loaded;
		}
		else
		{
			return SceneState::Unloaded;
		}
	}

	void SceneManager::RenderScene(RenderContext renderContext)
	{
		_scenes.top()->OnRender(renderContext);
		SDL_RenderPresent(renderContext);
	}

	void SceneManager::ProcessScene(TimeSpan elapsed)
	{
		_scenes.top()->OnProcessing(elapsed);
	}

	void SceneManager::HandleSceneEvents(SDL_Event& e)
	{
		auto scene = _scenes.top();

		switch(e.type)
		{
			case SDL_KEYDOWN:
			{
				scene->OnKeyDown(
					KeyEventArgs
					{
						.state = static_cast<ButtonState>(e.key.state),
						.key = e.key.keysym
					}
				);

				break;
			}

			case SDL_KEYUP:
			{
				scene->OnKeyUp(
					KeyEventArgs
					{
						.state = static_cast<ButtonState>(e.key.state),
						.key = e.key.keysym
					}
				);

				break;
			}

			case SDL_TEXTEDITING:
			{
				scene->OnTextChanged(
					TextChangedEventArgs
					{
						.text = e.edit.text,
						.selectionLength = static_cast<size_t>(e.edit.length),
						.selectionStart = e.edit.start
					}
				);

				break;
			}

			case SDL_TEXTEDITING_EXT:
			{
				scene->OnTextChanged(
					TextChangedEventArgs
					{
						.text = e.editExt.text,
						.selectionLength = static_cast<size_t>(e.editExt.length),
						.selectionStart = e.editExt.start
					}
				);
				SDL_free(e.editExt.text);

				break;
			}

			case SDL_TEXTINPUT:
			{
				scene->OnTextInput(
					TextInputEventArgs
					{
						.text = e.text.text
					}
				);

				break;
			}

			case SDL_MOUSEBUTTONDOWN:
			{
				scene->OnMouseDown(
					MouseButtonEventArgs
					{
						.button = static_cast<MouseButton>(e.button.button),
						.state = static_cast<ButtonState>(e.button.state),
						.clicksCount = e.button.clicks,
						.position =
						{
							.x = static_cast<float>(e.button.x),
							.y = static_cast<float>(e.button.y)
						}
					}
				);

				break;
			}

			case SDL_MOUSEBUTTONUP:
			{
				scene->OnMouseUp(
					MouseButtonEventArgs
					{
						.button = static_cast<MouseButton>(e.button.button),
						.state = static_cast<ButtonState>(e.button.state),
						.clicksCount = e.button.clicks,
						.position =
						{
							.x = static_cast<float>(e.button.x),
							.y = static_cast<float>(e.button.y)
						}
					}
				);

				break;
			}

			case SDL_MOUSEMOTION:
			{
				scene->OnMouseMove(
					MouseButtonEventArgs
					{
						.position =
						{
							.x = static_cast<float>(e.button.x),
							.y = static_cast<float>(e.button.y)
						}
					}
				);

				break;
			}

			case SDL_MOUSEWHEEL:
			{
				scene->OnMouseWheel(
					MouseWheelEventArgs
					{
						.position =
						{
							.x = static_cast<float>(e.button.x),
							.y = static_cast<float>(e.button.y)
						},
						.scrolledHorizontally = e.wheel.preciseX,
						.scrolledVertically = e.wheel.preciseY,
						.direction = SDL_MouseWheelDirection(e.wheel.direction)
					}
				);

				break;
			}
		}
	}

	void SceneManager::LoadScene()
	{
		_scenes.push(_scenesBuildersQueue.front()());
		_scenesBuildersQueue.pop();
	}

	void SceneManager::UnloadScene()
	{
		if(_scenesToUnload > 0)
		{
			_scenesToUnload--;
		}

		_scenes.pop();
	}
}