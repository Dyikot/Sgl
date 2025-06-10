#include "Scene.h"

namespace Sgl
{
	Scene::Scene():
		Class(*this),
		_layout(nullptr)
	{}

	void Scene::OnRender(RenderContext rc) const
	{
		auto texture = GetBackgroundTexture();

		if(texture)
		{
			rc.FillBackground(*texture, GetBackgroundColor());
		}
		else
		{
			rc.FillBackground(GetBackgroundColor());
		}

		_layout->Arrange();
		_layout->OnRender(rc);
	}

	void Scene::SetLayout(UI::Layout& layout)
	{
		_layout = &layout;
	}

	void Scene::OnMouseMove(const MouseButtonEventArgs& e)
	{
		_layout->OnSceneLayoutMouseMove(e);
	}

	void Scene::OnMouseDown(const MouseButtonEventArgs& e)
	{
		_layout->OnMouseDown(e);
	}

	void Scene::OnMouseUp(const MouseButtonEventArgs& e)
	{
		_layout->OnMouseUp(e);
	}

	void Scene::OnMouseWheel(const MouseWheelEventArgs& e)
	{
		_layout->OnMouseWheel(e);
	}

	void Scene::OnKeyDown(const KeyEventArgs& e)
	{
		_layout->OnKeyDown(e);
	}

	void Scene::OnKeyUp(const KeyEventArgs& e)
	{
		_layout->OnKeyUp(e);
	}
}