#include "Scene.h"
#include <cassert>

namespace Sgl
{
	Scene::Scene():
		_layout(nullptr)
	{}

	struct LayoutMouseLeave
	{
		Scene& Scene;

		void operator()(FPoint position) const
		{
			Cursor::Set(Scene.GetCursor());
		}
	};

	void Scene::SetLayout(shared_ptr<UI::Layout> value)
	{
		assert(value != nullptr);

		if(_layout)
		{
			_layout->MouseLeave -= LayoutMouseLeave(*this);
		}

		_layout = std::move(value);
		_layout->MouseLeave += LayoutMouseLeave(*this);
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