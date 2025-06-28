#include "Scene.h"
#include <cassert>

namespace Sgl
{
	Scene::Scene():
		Layout(nullptr)
	{}

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

		Layout->Arrange();
		Layout->OnRender(rc);
	}
}