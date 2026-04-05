#include "Renderable.h"
#include "../Application.h"
#include "../Base/Time/Stopwatch.h"

namespace Sgl
{
	Renderable::Renderable(Renderable&& other) noexcept:
		StyleableElement(std::move(other)),
		_visualRoot(std::exchange(other._visualRoot, nullptr)),
		_textures(std::exchange(other._textures, nullptr)),
		_cursor(other._cursor),
		_background(std::move(other._background))
	{}

	void Renderable::SetCursor(const Cursor& value, ValueSource source)
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
			Logging::LogDebug("[Backrgound] Source: {}", int(source));
			InvalidateRender();
			InvalidateBackground();
		}
	}

	void Renderable::SetVisualRoot(IVisualRoot* value)
	{
		_visualRoot = value;
		_textures = value ? &value->GetTextures() : nullptr;
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

	void Renderable::InvalidateRender()
	{
		if(_visualRoot)
		{
			_visualRoot->InvalidateRender();
		}
	}

	void Renderable::RenderBackground(RenderContext context)
	{
		if(_background.index() == 0)
		{
			context.FillBackground(std::get<Color>(_background));
		}
		else
		{
			if(_backgroundTexture == nullptr)
			{
				auto& path = std::get<ImagePath>(_background);	
				_backgroundTexture = _textures->Load(path);
			}

			context.DrawTexture(_backgroundTexture);
		}
	}

	void Renderable::RenderBackground(RenderContext context, const FRect& rect)
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
				auto& path = std::get<ImagePath>(_background);
				_backgroundTexture = _textures->Load(path);
				Logging::LogDebug("Texture loaded");
			}

			context.DrawTexture(_backgroundTexture, rect);
		}
	}

	void Renderable::InvalidateBackground()
	{
		_backgroundTexture = nullptr;
	}

	ResourceSetter<Renderable, const Brush&>::ResourceSetter(
		StyleableProperty<Renderable, const Brush&>& property, 
		ResourceKey key):
		SetterBase(property),
		_key(std::move(key))
	{}

	void ResourceSetter<Renderable, const Brush&>::Apply(
		StyleableElement& target, 
		ValueSource valueSource) const
	{
		auto& property = static_cast<StyleableProperty<Renderable, const Brush&>&>(GetProperty());
		property.InvokeSetter(
			static_cast<Renderable&>(target),
			App->Resources.GetBrush(_key.GetName()),
			valueSource
		);
	}
}
