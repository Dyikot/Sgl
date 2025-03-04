#include "Scene.h"
#include "../Window.h"

namespace Sgl
{
	Scene::Scene(Sgl::Window& window):
		Window(window)
	{}

	void Scene::OnRender(RenderContext& renderContext) const
	{
		if(BackgroundTexture)
		{
			renderContext.FillSceneBackgroundWithTexture(*BackgroundTexture, BackgroundColor);
		}
		else
		{
			renderContext.FillSceneBackgroundWithColor(BackgroundColor);
		}

		Components.OnRender(renderContext);
	}

	void Scene::SwitchCursorOn(const Cursor& cursor)
	{
		if(SDL_GetCursor() != cursor)
		{			
			SDL_SetCursor(cursor);
		}
	}

	void Scene::SwitchCursorOnDefault()
	{
		SwitchCursorOn(GetCursor());
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

	void Scene::OnLoaded(const EventArgs& e)
	{
		SwitchCursorOnDefault();

		Loaded.TryInvoke(this, e);
	}

	void Scene::OnUnloaded(const EventArgs& e)
	{
		Unloaded.TryInvoke(this, e);
	}

	SceneManager::SceneManager(Sgl::Window& window):
		Window(window)
	{}

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
		else if(!_scenesQueue.empty())
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

	void SceneManager::RenderScene()
	{
		auto& renderContext = Window.GetRenderContext();
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
						.State = static_cast<ButtonState>(e.key.state),
						.Key = e.key.keysym
					}
				);

				break;
			}

			case SDL_KEYUP:
			{
				scene->OnKeyUp(
					KeyEventArgs
					{
						.State = static_cast<ButtonState>(e.key.state),
						.Key = e.key.keysym
					}
				);

				break;
			}

			case SDL_TEXTEDITING:
			{
				scene->OnTextChanged(
					TextChangedEventArgs
					{
						.Text = e.edit.text,
						.SelectionLength = static_cast<size_t>(e.edit.length),
						.SelectionStart = e.edit.start
					}
				);

				break;
			}

			case SDL_TEXTEDITING_EXT:
			{
				scene->OnTextChanged(
					TextChangedEventArgs
					{
						.Text = e.editExt.text,
						.SelectionLength = static_cast<size_t>(e.editExt.length),
						.SelectionStart = e.editExt.start
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
						.Text = e.text.text
					}
				);

				break;
			}

			case SDL_MOUSEBUTTONDOWN:
			{
				scene->OnMouseDown(
					MouseButtonEventArgs
					{
						.Button = static_cast<MouseButton>(e.button.button),
						.State = static_cast<ButtonState>(e.button.state),
						.ClicksCount = e.button.clicks,
						.Position =
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
						.Button = static_cast<MouseButton>(e.button.button),
						.State = static_cast<ButtonState>(e.button.state),
						.ClicksCount = e.button.clicks,
						.Position =
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
						.Position =
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
						.Position =
						{
							.x = static_cast<float>(e.button.x),
							.y = static_cast<float>(e.button.y)
						},
						.ScrolledHorizontally = e.wheel.preciseX,
						.ScrolledVertically = e.wheel.preciseY,
						.Direction = SDL_MouseWheelDirection(e.wheel.direction)
					}
				);

				break;
			}
		}
	}

	void SceneManager::LoadScene()
	{
		_scenes.push(_scenesQueue.front());
		_scenes.top()->OnLoaded(EventArgs());
		_scenesQueue.pop();
	}

	void SceneManager::UnloadScene()
	{
		if(_scenesToUnload > 0)
		{
			_scenesToUnload--;
		}

		_scenes.top()->OnUnloaded(EventArgs());
		_scenes.pop();
	}
}