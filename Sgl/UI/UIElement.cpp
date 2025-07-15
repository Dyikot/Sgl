#include "UIElement.h"
#include "../Scene/Scene.h"

namespace Sgl
{
	UIElement::UIElement():
		Layoutable(),
		BackgroundColor(Colors::Transparent),
		Cursor(Cursors::Arrow),
		BackgroundTexture(),		
		Tag(),
		_isMouseOver(false)
	{}

	UIElement::UIElement(const UIElement& other):
		Layoutable(other),
		BackgroundColor(other.BackgroundColor),
		Cursor(other.Cursor),
		BackgroundTexture(other.BackgroundTexture),		
		Tag(other.Tag),
		_isMouseOver(other._isMouseOver)
	{}

	UIElement::UIElement(UIElement&& other) noexcept:
		Layoutable(std::move(other)),
		BackgroundColor(other.BackgroundColor),
		Cursor(other.Cursor),
		BackgroundTexture(std::move(other.BackgroundTexture)),		
		Tag(std::move(other.Tag)),
		_isMouseOver(other._isMouseOver)
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

	void Sgl::UIElement::OnRender(RenderContext context) const
	{		
		if(auto toolTip = ToolTip.Get(); toolTip != nullptr)
		{
			toolTip->OnRender(context);
		}
	}
}