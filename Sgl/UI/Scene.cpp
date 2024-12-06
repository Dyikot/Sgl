#include "Scene.h"
#include "../Application.h"

namespace Sgl
{
	Scene::Scene(const Style& style) noexcept:
		UIElement(style)
	{
		style.TryAddHandlerToEvent(LoadedProperty, Loaded);
		style.TryAddHandlerToEvent(UnloadedProperty, Unloaded);
		style.TryCopyTo(Control::BackgroundProperty, Background);
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

		if(_mouseOverPanel && TryUpdatePanelMouseMoveEvents(*_mouseOverPanel, e))
		{
			return;
		}

		for(auto panel : Panels)
		{
			if(panel == _mouseOverPanel)
			{
				continue;
			}

			if(TryUpdatePanelMouseMoveEvents(*panel, e))
			{
				_mouseOverPanel = panel;
				return;
			}
		}
	}

	bool Scene::TryUpdatePanelMouseMoveEvents(Panel& panel, const MouseButtonEventArgs& e)
	{
		if(auto child = panel.MouseOverControl; child && IsMouseOverControl(*child, e.Position))
		{
			child->OnMouseMove(e);

			if(child->ControlPanel != nullptr)
			{
				TryUpdatePanelMouseMoveEvents(*child->ControlPanel, e);
			}

			return true;
		}

		for(Control* child : panel.Children)
		{
			if(child == panel.MouseOverControl)
			{
				continue;
			}

			if(IsMouseOverControl(*child, e.Position))
			{
				if(panel.MouseOverControl != nullptr)
				{
					child->OnMouseLeave(e);
				}
				
				panel.MouseOverControl = child;
				child->OnMouseEnter(e);
				child->OnMouseMove(e);

				if(child->ControlPanel != nullptr)
				{
					TryUpdatePanelMouseMoveEvents(*child->ControlPanel, e);
				}

				return true;
			}
		}

		return false;
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