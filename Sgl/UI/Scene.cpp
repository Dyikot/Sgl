#include "Scene.h"
#include "../Application.h"

namespace Sgl
{
	Scene::Scene(const Style& style) noexcept:
		UIElement(style)
	{
		style.TryInit(Control::BackgroundProperty, Background);
		style.TryInitEvent(LoadedProperty, Loaded);
		style.TryInitEvent(UnloadedProperty, Unloaded);
	}

	void Scene::OnRender(RenderContext& renderContext)
	{
		renderContext.FillSceneBackground(Background);
	}

	void Scene::Close()
	{
		_isActive = false;
	}

	bool Scene::IsClosed() const
	{
		return !_isActive;
	}

	void Scene::OnMouseMove(const MouseButtonEventArgs& e)
	{
		UIElement::OnMouseMove(e);

		if(_mouseOverPanel && _mouseOverPanel->TryRaiseMouseMoveEvents(e))
		{
			return;
		}

		for(auto panel : Panels)
		{
			if(panel == _mouseOverPanel)
			{
				continue;
			}

			if(panel->TryRaiseMouseMoveEvents(e))
			{
				_mouseOverPanel = panel;
				return;
			}
		}
	}

	void Scene::OnMouseDown(const MouseButtonEventArgs& e)
	{
		UIElement::OnMouseDown(e);

		if(_mouseOverPanel)
		{
			_mouseOverPanel->RaiseMouseDownEvents(e);
		}
	}

	void Scene::OnMouseUp(const MouseButtonEventArgs& e)
	{
		UIElement::OnMouseUp(e);

		if(_mouseOverPanel)
		{
			_mouseOverPanel->RaiseMouseUpEvents(e);
		}
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