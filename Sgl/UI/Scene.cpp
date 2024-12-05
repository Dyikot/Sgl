#include "Scene.h"
#include "../Application.h"

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

	void Scene::OnMouseMove(const MouseButtonEventArgs& e)
	{
		UIElement::OnMouseMove(e);

		for(auto panel: Panels)
		{
			UpdatePanelMouseMoveEvents(*panel, e);
		}
	}

	void Scene::UpdatePanelMouseMoveEvents(Panel& panel, const MouseButtonEventArgs& e)
	{
		for(Control* child : *panel.Children)
		{
			if(IsMouseOverControl(*child, e.Position))
			{
				if(panel.MouseOverControl != nullptr)
				{
					child->OnMouseLeave(e);
				}

				panel.MouseOverControl = child;
				child->OnMouseEnter(e);
				child->OnMouseMove(e);
			}

			if(child->ControlPanel != nullptr)
			{
				UpdatePanelMouseMoveEvents(*child->ControlPanel, e);
			}
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