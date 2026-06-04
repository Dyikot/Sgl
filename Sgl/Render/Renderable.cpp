#include "Renderable.h"
#include "../Application.h"

namespace Sgl
{
	class ColorBackgroundRenderer final : public IBackgroundRenderer
	{
	public:
		explicit ColorBackgroundRenderer(Color color): _color(color) {}

		void Render(RenderContext context) const
		{
			context.FillBackground(_color);
		}

		void Render(RenderContext context, const FRect& rect) const
		{
			context.DrawRectangleFill(rect, _color);
		}
	private:
		Color _color;
	};

	class TextureBackgroundRenderer final : public IBackgroundRenderer
	{
	public:
		explicit TextureBackgroundRenderer(const Texture& texture): _texture(texture) {}

		void Render(RenderContext context) const
		{
			context.DrawTexture(_texture, nullptr, nullptr);
		}

		void Render(RenderContext context, const FRect& rect) const
		{
			context.DrawTexture(_texture, &rect, nullptr);
		}
	private:
		Texture _texture;
	};

	Renderable::Renderable()
	{
		UpdateBackgroundRenderer();
	}

	Renderable::Renderable(Renderable&& other) noexcept:
		StyleableElement(std::move(other)),
		_visualRoot(std::exchange(other._visualRoot, nullptr)),
		_backgroundRenderer(std::move(other._backgroundRenderer)),
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
			UpdateBackgroundRenderer();
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

	void Renderable::Render(RenderContext context)
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

	void Renderable::RenderBackground(RenderContext context)
	{
		_backgroundRenderer->Render(context);
	}

	void Renderable::RenderBackground(RenderContext context, const FRect& rect)
	{
		_backgroundRenderer->Render(context, rect);
	}

	void Renderable::UpdateBackgroundRenderer()
	{
		if(_background.index() == 0)
		{
			Color color = std::get<Color>(_background);
			_backgroundRenderer = std::make_unique<ColorBackgroundRenderer>(color);
		}
		else
		{
			auto& source = std::get<ImageSource>(_background);
			Texture texture = GetVisualRoot()->GetTextureFactory().Create(source, false);
			_backgroundRenderer = std::make_unique<TextureBackgroundRenderer>(texture);
		}
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
