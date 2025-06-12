#include "Scene.h"
#include <cassert>

namespace Sgl
{
	Scene::Scene():
		Class(*this),
		_layout(nullptr)
	{}

	void Scene::SetLayout(shared_ptr<UI::Layout> value)
	{
		assert(value != nullptr);
		_layout = std::move(value);
		_layout->Parent = this;
	}

	shared_ptr<UI::Layout> Scene::GetLayout() const
	{
		return _layout;
	}

	void Scene::OnRender(RenderContext rc) const
	{
		auto texture = GetBackgroundTexture();
		auto color = GetBackgroundColor();

		if(texture)
		{
			rc.DrawTexture(*texture, color);
		}
		else
		{
			rc.FillBackground(color);
		}

		_layout->Arrange();
		_layout->OnRender(rc);
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