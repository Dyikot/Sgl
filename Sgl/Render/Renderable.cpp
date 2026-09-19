#include "Renderable.h"
#include "../Application.h"

namespace Sgl
{
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
		ResourceKey key):
		_property(property),
		_key(std::move(key))
	{}

	void ResourceSetter<Renderable, const Brush&>::Apply(
		Styleable& target, 
		ValueSource valueSource) const
	{
		auto& owner = static_cast<Renderable&>(target);
		_property.InvokeSetter(owner, App->Resources.GetBrush(_key.Value), valueSource);
	}

	ISavedValue* ResourceSetter<Renderable, const Brush&>::Save(Styleable& target) const
	{
		auto& owner = static_cast<Renderable&>(target);
		return new SavedPropertyValue<Renderable, const Brush&>(_property, owner);
	}
}
