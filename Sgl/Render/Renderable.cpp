#include "Renderable.h"

namespace Sgl
{
	Renderable::Renderable(const Renderable& other):
		StyleableElement(other),
		_visualRoot(other._visualRoot),
		_cursor(other._cursor),
		_background(other._background)
	{}

	Renderable::Renderable(Renderable&& other) noexcept:
		StyleableElement(std::move(other)),
		_visualRoot(std::exchange(other._visualRoot, nullptr)),
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
			InvalidateRender();
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
}
