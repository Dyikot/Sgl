#include "Renderable.h"

namespace Sgl
{
	Renderable::Renderable(const Renderable& other):
		StyleableElement(other),
		_visualRoot(other._visualRoot),
		_cursor(other._cursor),
		_background(other._background),
		_isRenderValid(other._isRenderValid)
	{}

	Renderable::Renderable(Renderable&& other) noexcept:
		StyleableElement(std::move(other)),
		_visualRoot(std::exchange(other._visualRoot, nullptr)),
		_cursor(other._cursor),
		_background(std::move(other._background)),
		_isRenderValid(other._isRenderValid)
	{}

	void Renderable::SetCursor(const Cursor& value)
	{
		SetProperty(CursorProperty, _cursor, value);
		OnCursorChanged(value);
	}

	void Renderable::SetBackground(const Brush& value)
	{
		SetProperty(BackgroundProperty, _background, value);
		InvalidateRender();
	}

	void Renderable::SetVisualRoot(IVisualRoot* value)
	{
		_visualRoot = value;
	}

	void Renderable::SetParent(StyleableElement* parent)
	{
		StyleableElement::SetParent(parent);

		if(parent == nullptr)
		{
			_visualParent = nullptr;
			return;
		}

		if(auto visualParent = dynamic_cast<Renderable*>(parent))
		{
			_visualParent = visualParent;
		}
	}

	void Renderable::Render(RenderContext context)
	{
		_isRenderValid = true;
	}

	void Renderable::InvalidateRender()
	{
		if(_isRenderValid)
		{
			_isRenderValid = false;

			if(_visualParent)
			{
				_visualParent->InvalidateRender();
			}
		}
	}
}
