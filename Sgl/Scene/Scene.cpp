#include "Scene.h"
#include "../Application.h"

namespace Sgl
{
	Scene::Scene():
		BackgroundColor(Colors::Black),
		_uielement(nullptr),
		_stylingParent(App.Current())
	{}

	void Scene::SetUIElement(shared_ptr<UIElement> value)
	{
		_uielement = value;
		_uielement->_stylingParent = this;
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
		
		_uielement->OnRender(context);
	}

	void Scene::Process(TimeSpan elapsed)
	{
		if(_uielement)
		{
			UpdateInvalidatedLayout();
		}
	}

	void Scene::OnCreated()
	{
		if(_uielement)
		{
			UpdateInvalidatedLayout();
		}
	}

	void Scene::UpdateInvalidatedLayout()
	{
		if(!_uielement->IsStyleValid())
		{
			_uielement->ApplyStyle();
		}

		if(!_uielement->IsMeasureValid())
		{
			auto& window = App->Window;
			auto availableSize = FSize(window.GetWidth(), window.GetHeight());
			_uielement->Measure(availableSize);
		}

		if(!_uielement->IsArrangeValid())
		{
			auto& window = App->Window;
			auto availableSize = FSize(window.GetWidth(), window.GetHeight());
			auto bounds = FRect(0, 0, availableSize.Width, availableSize.Height);
			_uielement->Arrange(bounds);
		}
	}
}