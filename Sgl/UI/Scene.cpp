#include "Scene.h"
#include "../Application.h"
#include "Controls/Control.h"

namespace Sgl
{
	Scene::Scene(const Style& style) noexcept:
		UIElement(style)
	{
		style.TryInit(LoadedProperty, Loaded);
		style.TryInit(UnloadedProperty, Unloaded);
		style.TryInit(Control::BackgroundProperty, Background);
	}

	void Scene::OnRender(RenderContext& renderContext) const
	{
		renderContext.DrawSceneBackground(Background);
	}

	void Scene::Close()
	{
		_isClosed = true;
	}

	bool Scene::IsClosed() const noexcept
	{
		return _isClosed;
	}

	Control* Scene::FindControl(SDL_Point mousePosition)
	{
		
	}

	void Scene::OnLoaded(const EventArgs& e)
	{
		if(Loaded)
		{
			Loaded(this, e);
		}
	}

	void Scene::OnUnloaded(const EventArgs& e)
	{
		if(Unloaded)
		{
			Unloaded(this, e);
		}
	}
}