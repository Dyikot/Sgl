#include "Scene.h"
#include "../Application.h"

namespace Sgl
{
	void SceneBackgroundDrawing::Draw() const
	{
		SetRenderColor(Background);
		Fill(Background);
	}

	void SceneBackgroundDrawing::Fill(ColorBrush brush) const
	{
		SDL_RenderClear(Renderer);
	}

	void SceneBackgroundDrawing::Fill(ImageBrush& brush) const
	{
		SDL_RenderCopy(Renderer, brush.Image, NULL, NULL);
	}

	void Scene::SetStyle(const Style& style)
	{
		UIElement::SetStyle(style);
		style.TryInit(ClosedProperty, Closed);
		style.TryInit(BackgroundProperty, Background);
	}

	void Scene::OnRender() const
	{
		_backgroundDrawing.Draw();
	}

	void Scene::HandleEvent(const SDL_Event& e)
	{
		switch(e.type)
		{
			case SDL_KEYDOWN:
			{
				OnKeyDown(KeyEventArgs
					{
						.IsDown = true,
						.IsUp = false,
						.Key = e.key.keysym
					});

				break;
			}

			case SDL_KEYUP:
			{
				OnKeyUp(KeyEventArgs
					{
						.IsDown = false,
						.IsUp = true,
						.Key = e.key.keysym
					});

				break;
			}

			case SDL_MOUSEBUTTONDOWN:
			{
				OnMouseDown(MouseButtonEventArgs
					{
						.Button = ToMouseButton(e.button.button),
						.ButtonState = ToMouseButtonState(e.button.state),
						.ClickCount = e.button.clicks,
						.Position = { e.button.x, e.button.y }
					});

				break;
			}

			case SDL_MOUSEBUTTONUP:
			{
				OnMouseUp(MouseButtonEventArgs
					{
						.Button = ToMouseButton(e.button.button),
						.ButtonState = ToMouseButtonState(e.button.state),
						.ClickCount = e.button.clicks,
						.Position = { e.button.x, e.button.y }
					});

				break;
			}

			case SDL_MOUSEMOTION:
			{
				OnMouseMove(MouseButtonEventArgs{ .Position = { e.motion.x, e.motion.y } });
				break;
			}
		}
	}

	void Scene::Close()
	{	
		OnClose(EventArgs{});
		Application::Current()->GetWindow()->Scenes.RemoveCurrent();
	}
	
	void Scene::OnClose(const EventArgs & e)
	{
		if(Closed)
		{
			Closed(this, e);
		}
	}
	
}