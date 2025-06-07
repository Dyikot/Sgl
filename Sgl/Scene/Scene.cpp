#include "Scene.h"

namespace Sgl
{
	Scene::Scene(std::unique_ptr<UI::Layout> layout):
		Class(*this),
		Layout(std::move(layout))
	{
		if(Layout == nullptr)
		{
			throw std::invalid_argument("Layout cannot be null");
		}
	}

	Scene::Scene(Func<std::unique_ptr<UI::Layout>> layoutFactory):
		Scene(layoutFactory())
	{}

	void Scene::OnRender(RenderContext rc) const
	{
		if(BackgroundTexture)
		{
			rc.FillBackground(*BackgroundTexture, BackgroundColor);
		}
		else
		{
			rc.FillBackground(BackgroundColor);
		}

		Layout->OnRender(rc);
	}
}