#include "Scene.h"

namespace Sgl
{
	Scene::Scene(std::unique_ptr<UI::Layout> layout):
		Class(*this),
		Layout(std::move(layout))
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