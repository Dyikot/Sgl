#include "Scene.h"
#include "../Application.h"
#include "../Base/Math.h"

namespace Sgl
{
	Scene::Scene()
	{
		SetBackground(Colors::White);
	}

	void Scene::SetContent(Ref<UIElement> value)
	{
		if(_content)
		{
			_content->_parent = nullptr;
		}

		SetProperty(ContentProperty, _content, std::move(value));

		if(_content)
		{
			_content->_parent = this;
		}
	}

	void Scene::Render(RenderContext context)
	{	
		switch(auto background = GetBackground(); background.GetType())
		{
			case Brush::Color:
				context.SetBackground(background.AsColor()); break;
			case Brush::Texture:
				context.DrawTexture(background.AsTexture()); break;
		}

		if(_content && _content->IsVisible())
		{
			_content->Render(context);
		}

		Renderable::Render(context);
	}

	void Scene::Process(TimeSpan elapsed)
	{
		UpdateStyleAndLayout();
	}

	void Scene::OnCursorChanged(const Cursor& cursor)
	{
		if(!(_content && _content->IsMouseOver()))
		{
			Cursor::Set(cursor);
		}
	}

	void Scene::OnCreated()
	{
		UpdateStyleAndLayout();
	}

	void Scene::OnMouseMove(MouseEventArgs& e)
	{
		if(_content && _content->IsVisible())
		{
			auto& content = _content.GetValue();
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
				Cursor::Set(GetCursor());
			}
		}
	}

	void Scene::OnMouseDown(MouseButtonEventArgs& e)
	{
		if(_content && _content->IsMouseOver() && _content->IsVisible())
		{
			_content->OnMouseDown(e);
		}
	}

	void Scene::OnMouseUp(MouseButtonEventArgs& e)
	{
		if(_content && _content->IsMouseOver() && _content->IsVisible())
		{
			_content->OnMouseUp(e);
		}
	}

	void Scene::UpdateStyleAndLayout()
	{
		if(!IsStyleValid())
		{
			ApplyStyle();
		}

		if(_content)
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
}