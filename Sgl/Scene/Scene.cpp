#include "Scene.h"
#include "../Application.h"
#include "../Base/Math.h"

namespace Sgl
{
	Scene::Scene():
		BackgroundColor(Colors::Black),
		_uielement(nullptr),
		_stylingParent(App.Current())
	{}

	void Scene::SetUIElement(std::shared_ptr<UIElement> value)
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
		
		if(_uielement && _uielement->IsVisible)
		{
			_uielement->OnRender(context);
		}
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

	void Scene::OnKeyUp(const KeyEventArgs& e)
	{
		
	}

	void Scene::OnKeyDown(const KeyEventArgs& e)
	{
		
	}

	void Scene::OnMouseMove(const MouseEventArgs& e)
	{
		if(_uielement && _uielement->IsVisible)
		{
			auto& element = *_uielement;
			bool wasMouseOver = element._isMouseOver;
			bool isMouseOver = Math::IsPointInRect(e.Position, element._bounds);

			if(isMouseOver)
			{
				if(!wasMouseOver)
				{
					element.OnMouseEnter(e);
				}

				element.OnMouseMove(e);
			}
			else if(wasMouseOver)
			{
				element.OnMouseLeave(e);
				Cursor::Set(Cursor);
			}
		}
	}

	void Scene::OnMouseDown(const MouseButtonEventArgs& e)
	{
		if(_uielement && _uielement->IsMouseOver() && _uielement->IsVisible)
		{
			_uielement->OnMouseDown(e);
		}
	}

	void Scene::OnMouseUp(const MouseButtonEventArgs& e)
	{
		if(_uielement && _uielement->IsMouseOver() && _uielement->IsVisible)
		{
			_uielement->OnMouseUp(e);
		}
	}

	void Scene::OnMouseWheelChanged(const MouseWheelEventArgs& e)
	{
		
	}

	void Scene::OnTextInput(const TextInputEventArgs& e)
	{
		
	}

	void Scene::OnTextEditing(const TextEditingEventArgs& e)
	{
		
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