#include "Renderable.h"
#include "../Application.h"

namespace Sgl
{
	void Renderable::SetCursor(Cursor value, ValueSource source)
	{
		SetProperty(CursorProperty, _cursor, value, _cursorSource, source);
	}

	Cursor Renderable::GetCursor() const
	{
		return GetProperty(CursorProperty, _cursor);
	}

	void Renderable::SetBackground(const Brush& value, ValueSource source)
	{
		SetProperty(BackgroundProperty, _background, value, _backgroundSource, source);
	}

	const Brush& Renderable::GetBackground() const
	{
		return GetProperty(BackgroundProperty, _background);
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

	void Renderable::OnPropertyChanged(PropertyBase& property)
	{
		Styleable::OnPropertyChanged(property);

		if(property == BackgroundProperty)
		{
			_isBackgroundTransparent = _background == Colors::Transparent;
			InvalidateRender();
			OnBackgroundChanged(_background);
		}
		else if(property == CursorProperty)
		{
			OnCursorChanged(_cursor);
		}
	}

	void Renderable::SetVisualRoot(IVisualRoot* visualRoot)
	{
		_visualRoot = visualRoot;
	}

	void Renderable::OnAttachedToLogicalTree()
	{
		if(auto renderable = dynamic_cast<Renderable*>(GetStylingParent()))
		{
			_visualRoot = renderable->GetVisualRoot();
		}

		Styleable::OnAttachedToLogicalTree();
	}

	void Renderable::OnDetachedFromLogicalTree()
	{
		Styleable::OnDetachedFromLogicalTree();
		_visualRoot = nullptr;
	}

	ResourceSetter<Renderable, const Brush&>::ResourceSetter(
		BackgroundProperty& property,
		std::string key):
		_property(property),
		_key(std::move(key))
	{}

	PropertyBase& ResourceSetter<Renderable, const Brush&>::GetProperty() const
	{
		return _property;
	}

	void ResourceSetter<Renderable, const Brush&>::Apply(
		Styleable& target, 
		ValueSource valueSource) const
	{
		auto& owner = static_cast<Renderable&>(target);
		_property.InvokeSetter(owner, App->Resources.GetBrush(_key), valueSource);
	}
}
