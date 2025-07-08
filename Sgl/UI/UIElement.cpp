#include "UIElement.h"
#include "../Scene/Scene.h"

namespace Sgl
{
	UIElement::UIElement():
		BackgroundColor(Colors::Black),
		Cursor(Cursors::Arrow),
		BackgroundTexture(),
		Width(*this),
		Height(*this),
		MinWidth(),
		MinHeight(),
		MaxWidth(std::numeric_limits<float>::max()),
		MaxHeight(std::numeric_limits<float>::max()),
		IsVisible(true),
		ZIndex(1),
		Margin(),
		VerticalAlignment(VerticalAlignment::Top),
		HorizontalAlignment(HorizontalAlignment::Left),
		Tag(),
		_actualWidth(Width - Margin->Right),
		_actualHeight(Height - Margin->Bottom),
		_position(),
		_actualPosition(_position.x + Margin->Left, _position.y + Margin->Right)
	{}

	UIElement::UIElement(const UIElement& other):
		BackgroundColor(other.BackgroundColor),
		Cursor(other.Cursor),
		BackgroundTexture(other.BackgroundTexture),
		Width(other.Width),
		Height(other.Height),
		MinWidth(other.MinWidth),
		MinHeight(other.MinHeight),
		MaxWidth(other.MaxWidth),
		MaxHeight(other.MaxHeight),
		IsVisible(other.IsVisible),
		ZIndex(other.ZIndex),
		Margin(other.Margin),
		VerticalAlignment(other.VerticalAlignment),
		HorizontalAlignment(other.HorizontalAlignment),
		Tag(other.Tag),
		_actualWidth(other._actualWidth),
		_actualHeight(other._actualHeight),
		_position(other._position),
		_actualPosition(other._actualPosition)
	{}

	UIElement::UIElement(UIElement&& other) noexcept:
		BackgroundColor(std::move(other.BackgroundColor)),
		Cursor(std::move(other.Cursor)),
		BackgroundTexture(std::move(other.BackgroundTexture)),
		Width(std::move(other.Width)),
		Height(std::move(other.Height)),
		MinWidth(std::move(other.MinWidth)),
		MinHeight(std::move(other.MinHeight)),
		MaxWidth(std::move(other.MaxWidth)),
		MaxHeight(std::move(other.MaxHeight)),
		IsVisible(std::move(other.IsVisible)),
		ZIndex(std::move(other.ZIndex)),
		Margin(std::move(other.Margin)),
		VerticalAlignment(std::move(other.VerticalAlignment)),
		Tag(std::move(other.Tag)),
		HorizontalAlignment(std::move(other.HorizontalAlignment)),
		_actualWidth(other._actualWidth),
		_actualHeight(other._actualHeight),
		_position(other._position),
		_actualPosition(other._actualPosition)
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

	void UIElement::SetTooltip(const ToolTip& tooltip)
	{}

	void Sgl::UIElement::OnRender(RenderContext context) const
	{		
		
	}
}