#include "Scene.h"
#include "../Application.h"
#include "../Base/Math.h"

namespace Sgl
{
	Scene::Scene():
		_cursor(Cursors::Arrow),
		_background(Colors::Black),
		_content(),
		_stylingParent(App.Current())
	{}

	void Scene::SetCursor(const Cursor& value)
	{
		_cursor = value;

		if(!(_content && _content->IsMouseOver()))
		{
			Cursor::Set(value);
		}
	}

	void Scene::SetContent(Unique<UIElement> value)
	{
		if(_content)
		{
			_content->_stylingParent = nullptr;
		}

		_content = std::move(value);

		if(_content)
		{
			_content->_stylingParent = this;
		}
	}

	void Scene::Render(RenderContext context)
	{		
		switch(_background.GetType())
		{
			case Brush::Color:
				context.FillBackground(_background.AsColor()); break;
			case Brush::Texture:
				context.DrawTexture(*_background.AsTexture()); break;
		}

		if(_content && _content->IsVisible())
		{
			_content->Render(context);
		}
	}

	void Scene::Process(TimeSpan elapsed)
	{
		if(_content)
		{
			UpdateLayoutAndStyle();
		}
	}

	void Scene::OnCreated()
	{
		if(_content)
		{
			UpdateLayoutAndStyle();
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
		if(_content && _content->IsVisible())
		{
			auto& content = *_content;
			bool wasMouseOver = content._isMouseOver;
			bool isMouseOver = Math::IsPointInRect(e.Position, content._bounds);

			if(isMouseOver)
			{
				if(!wasMouseOver)
				{
					content.OnMouseEnter(e);
				}

				content.OnMouseMove(e);
			}
			else if(wasMouseOver)
			{
				content.OnMouseLeave(e);
				Cursor::Set(_cursor);
			}
		}
	}

	void Scene::OnMouseDown(const MouseButtonEventArgs& e)
	{
		if(_content && _content->IsMouseOver() && _content->IsVisible())
		{
			_content->OnMouseDown(e);
		}
	}

	void Scene::OnMouseUp(const MouseButtonEventArgs& e)
	{
		if(_content && _content->IsMouseOver() && _content->IsVisible())
		{
			_content->OnMouseUp(e);
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

	void Scene::UpdateLayoutAndStyle()
	{
		if(!_content->IsStyleValid())
		{
			_content->ApplyStyle();
		}

		if(!_content->IsMeasureValid())
		{
			auto [width, height] = App->Window.GetSize();
			_content->Measure(FSize(width, height));
		}

		if(!_content->IsArrangeValid())
		{
			auto [width, height] = App->Window.GetSize();
			_content->Arrange(FRect(0, 0, width, height));
		}
	}
}