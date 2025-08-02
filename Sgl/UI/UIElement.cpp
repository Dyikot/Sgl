#include "UIElement.h"
#include "../Scene/Scene.h"

namespace Sgl
{
	UIElement::UIElement():
		Layoutable(),
		DataContext(),
		_isMouseOver(false),
		_cursor(CursorProperty.DefaultValue),
		_backgroundTexture(),
		_backgroundColor(BackgroundColorProperty.DefaultValue),
		_tag(),
		_toolTip(),
		_zIndex()
	{}

	UIElement::UIElement(const UIElement& other):
		Layoutable(other),
		DataContext(other.DataContext),
		_isMouseOver(other._isMouseOver),
		_cursor(other._cursor),
		_backgroundTexture(other._backgroundTexture),
		_backgroundColor(other._backgroundColor),
		_tag(other._tag),
		_toolTip(other._toolTip),
		_zIndex(other._zIndex)
	{}

	UIElement::UIElement(UIElement&& other) noexcept:
		Layoutable(std::move(other)),
		DataContext(std::move(other.DataContext)),
		_isMouseOver(other._isMouseOver),
		_cursor(other._cursor),
		_backgroundTexture(std::move(other._backgroundTexture)),
		_backgroundColor(other._backgroundColor),
		_tag(std::move(other._tag)),
		_toolTip(std::move(other._toolTip)),
		_zIndex(other._zIndex)
	{}

	Scene& UIElement::GetScene()
	{
		Scene* scene = nullptr;

		while(scene == nullptr)
		{
			scene = dynamic_cast<Scene*>(_stylingParent);
		}

		return *scene;
	}

	void UIElement::Render(RenderContext context) const
	{		
		if(_toolTip && _toolTip->IsVisible())
		{
			_toolTip->Render(context);
		}
	}
}