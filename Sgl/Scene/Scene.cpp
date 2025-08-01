#include "Scene.h"
#include "../Application.h"
#include "../Base/Math.h"

namespace Sgl
{
	Scene::Scene():
		_cursor(Cursors::Arrow),
		_background(Colors::Black, nullptr),
		_content(nullptr),
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

	void Scene::SetBackground(Color value)
	{
		_background.Color = value;
	}

	void Scene::SetBackground(Shared<Texture> value)
	{
		_background.Texture = std::move(value);
	}

	void Scene::SetBackground(Color color, Shared<Texture> texture)
	{
		_background = Background(color, std::move(texture));
	}

	void Scene::SetContent(Shared<UIElement> value)
	{
		_content = std::move(value);
		_content->_stylingParent = this;
	}

	void Scene::Render(RenderContext context) const
	{
		if(_background.Texture)
		{
			context.DrawTexture(*_background.Texture, _background.Color);
		}
		else
		{
			context.FillBackground(_background.Color);
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
			UpdateInvalidatedLayoutAndStyle();
		}
	}

	void Scene::OnCreated()
	{
		if(_content)
		{
			UpdateInvalidatedLayoutAndStyle();
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

	void Scene::UpdateInvalidatedLayoutAndStyle()
	{
		if(!_content->IsStyleValid())
		{
			_content->ApplyStyle();
		}

		if(!_content->IsMeasureValid())
		{
			auto& window = App->Window;
			auto availableSize = FSize(window.GetWidth(), window.GetHeight());
			_content->Measure(availableSize);
		}

		if(!_content->IsArrangeValid())
		{
			auto& window = App->Window;
			auto availableSize = FSize(window.GetWidth(), window.GetHeight());
			auto bounds = FRect(0, 0, availableSize.Width, availableSize.Height);
			_content->Arrange(bounds);
		}
	}
}