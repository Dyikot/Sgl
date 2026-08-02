#include "Renderable.h"
#include "../Application.h"

namespace Sgl
{
	Renderable::Renderable(Renderable&& other) noexcept:
		Styleable(std::move(other)),
		_visualRoot(std::exchange(other._visualRoot, nullptr)),
		_cursor(std::move(other._cursor)),
		_background(std::move(other._background)),
		_isBackgroundTransparent(other._isBackgroundTransparent),
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
			_isBackgroundTransparent = value == Colors::Transparent;
			InvalidateRender();
			OnBackgroundChanged(_background);
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
			_visualRoot->MarkDirty();
			_isDirty = true;
		}
	}

	void Renderable::SetVisualRoot(IVisualRoot* visualRoot)
	{
		_visualRoot = visualRoot;
	}

	void Renderable::OnAttachedToLogicalTree()
	{
		Styleable::OnAttachedToLogicalTree();

		if(auto renderable = dynamic_cast<Renderable*>(GetStylingParent()))
		{
			_visualRoot = renderable->GetVisualRoot();
		}
		else
		{
			_visualRoot = nullptr;
		}
	}

	void Renderable::OnDetachedFromLogicalTree()
	{
		Styleable::OnDetachedFromLogicalTree();
		_visualRoot = nullptr;
	}

	ResourceSetter<Renderable, const Brush&>::ResourceSetter(
		BackgroundProperty& property,
		ResourceKey key):
		Setter(property),
		_key(std::move(key))
	{}

	void ResourceSetter<Renderable, const Brush&>::Apply(
		Styleable& target, 
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
