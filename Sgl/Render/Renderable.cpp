#include "Renderable.h"
#include "../Application.h"

namespace Sgl
{
	Renderable::Renderable(Renderable&& other) noexcept:
		StyleableElement(std::move(other)),
		_visualRoot(std::exchange(other._visualRoot, nullptr)),
		_cursor(other._cursor),
		_background(other._background),
		_isDirty(other._isDirty)
	{}

	void Renderable::SetCursor(Cursor value, ValueSource source)
	{
		if(SetProperty(CursorProperty, _cursor, value, _cursorSource, source))
		{
			OnCursorChanged(value);
		}
	}

	void Renderable::SetBackground(const Brush& value, ValueSource source)
	{
		if(SetProperty(BackgroundProperty, _background, value, _backgroundSource, source))
		{
			InvalidateRender();
			InvalidateBackground();
		}
	}

	void Renderable::SetVisualRoot(IVisualRoot* value)
	{
		_visualRoot = value;
	}

	void Renderable::SetParent(IStyleHost* parent)
	{
		StyleableElement::SetParent(parent);

		if(parent == nullptr)
		{
			SetVisualRoot(nullptr);
			return;
		}

		if(auto renderable = dynamic_cast<Renderable*>(parent))
		{
			if(auto visualRoot = renderable->GetVisualRoot())
			{
				SetVisualRoot(visualRoot);
			}
		}
	}

	void Renderable::Render(RenderContext& context)
	{
		_isDirty = false;
	}

	void Renderable::InvalidateRender()
	{
		if(_visualRoot && !_isDirty)
		{
			_visualRoot->InvalidateRender();
			_isDirty = true;
		}
	}

	void Renderable::RenderBackground(RenderContext& context)
	{
		if(_background.index() == 0)
		{
			context.FillBackground(std::get<Color>(_background));
		}
		else
		{
			if(_backgroundTexture == nullptr)
			{
				auto& source = std::get<ImageSource>(_background);	
				_backgroundTexture = context.LoadTexture(source);
			}

			context.DrawTexture(_backgroundTexture);
		}
	}

	void Renderable::RenderBackground(RenderContext& context, const FRect& rect)
	{
		if(_background.index() == 0)
		{
			Color color = std::get<Color>(_background);

			if(color != Colors::Transparent)
			{
				context.DrawRectangleFill(rect, color);
			}
		}
		else
		{
			if(_backgroundTexture == nullptr)
			{
				auto& source = std::get<ImageSource>(_background);
				_backgroundTexture = context.LoadTexture(source);
			}

			context.DrawTexture(_backgroundTexture, rect);
		}
	}

	void Renderable::InvalidateBackground()
	{
		_backgroundTexture = nullptr;
	}

	ResourceSetter<Renderable, const Brush&>::ResourceSetter(
		BackgroundProperty& property,
		ResourceKey key):
		SetterBase(property),
		_key(std::move(key))
	{}

	void ResourceSetter<Renderable, const Brush&>::Apply(
		StyleableElement& target, 
		ValueSource valueSource) const
	{
		auto& property = static_cast<BackgroundProperty&>(GetProperty());
		property.InvokeSetter(
			static_cast<Renderable&>(target),
			App->Resources.GetBrush(_key.Value),
			valueSource
		);
	}
}
