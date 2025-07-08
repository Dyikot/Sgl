#include "Scene.h"
#include "../Application.h"

namespace Sgl
{
	Scene::Scene():
		BackgroundColor(Colors::Black),
		_layout(nullptr),
		_stylingParent(App.Current())
	{}

	void Scene::SetLayout(shared_ptr<Layout> value)
	{
		_layout = value;
		_layout->_stylingParent = this;
	}

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
		
		_layout->OnRender(context);
	}

	void Scene::Process(TimeSpan elapsed)
	{
		_layout->Arrange();
	}

	void Scene::OnCreated()
	{
		_layout->ApplyStyle();
	}
}