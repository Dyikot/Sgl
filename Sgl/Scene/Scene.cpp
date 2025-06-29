#include "Scene.h"
#include <cassert>

namespace Sgl
{
	Scene::Scene():
		BackgroundColor(Colors::Black),
		Layout(nullptr)
	{}

	void Scene::OnRender(RenderContext rc) const
	{
		if(BackgroundTexture)
		{
			rc.DrawTexture(*BackgroundTexture, BackgroundColor);
		}
		else
		{
			rc.FillBackground(BackgroundColor);
		}
		
		Layout->OnRender(rc);
	}

	void Scene::Process(TimeSpan elapsed)
	{
		Layout->Arrange();
	}

	void Scene::OnCreated()
	{
		Layout->ApplyStyle();
	}
}