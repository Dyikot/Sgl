#include "Scene.h"

namespace Sgl
{
	Scene::Scene():
		BackgroundColor(Colors::Black),
		Layout(nullptr)
	{}

	void Sgl::Scene::OnRender(RenderContext context) const
	{
		if(BackgroundTexture)
		{
			context.DrawTexture(*BackgroundTexture, BackgroundColor);
		}
		else
		{
			context.FillBackground(BackgroundColor);
		}
		
		Layout->OnRender(context);
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