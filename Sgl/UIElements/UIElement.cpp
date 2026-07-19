#include "UIElement.h"
#include "../Application.h"
#include "../Layout/LayoutHelper.h"
#include <cassert>
#include <ranges>

namespace Sgl
{
	UIElement::UIElement(UIElement&& other) noexcept:
		Layoutable(std::move(other)),
		_backgroundFragment(std::move(other._backgroundFragment)),
		_tag(std::move(other._tag)),
		_cornersRadius(other._cornersRadius),
		_tagSource(other._tagSource),
		_cornersRadiusSource(other._cornersRadiusSource)
	{}

	void UIElement::SetTag(const Any& value, ValueSource source)
	{
		SetProperty(TagProperty, _tag, value, _tagSource, source);
	}

	void UIElement::SetCornersRadius(float value, ValueSource source)
	{
		if(SetProperty(CornersRadiusProperty, _cornersRadius, value, _cornersRadiusSource, source))
		{
			InvalidateRender();
			InvalidateBackground();
		}
	}

	Ref<UIElement> UIElement::HitTest(const Ref<UIElement>& self, FPoint point)
	{
		Ref<UIElement> hit;

		for(auto& child : self->GetChildren() | std::views::reverse)
		{
			hit = HitTest(child, point);

			if(hit)
			{
				return hit;
			}
		}

		return self->IsVisible() && IsPointInRect(point.x, point.y, self->GetBounds()) ? self : nullptr;
	}

	void UIElement::Render(RenderContext context)
	{
		Renderable::Render(context);

		if(!IsVisible())
		{
			return;
		}

		if(!IsBackgroundTransparent())
		{
			if(!_backgroundFragment)
			{
				_backgroundFragment = CreateBackgroundFragment(GetBackground());
			}

			_backgroundFragment(context, GetBounds());
		}

		for(auto& child : GetChildren())
		{
			child->Render(context);
		}
	}

	void UIElement::SetParent(IStyleHost* parent)
	{
		Layoutable::SetParent(parent);
		_parent = dynamic_cast<UIElement*>(parent);
	}

	void UIElement::OnCursorChanged(Cursor cursor)
	{
		Renderable::OnCursorChanged(cursor);

		if(IsMouseOver())
		{
			SetCurrentCursor(cursor);
		}
	}

	void UIElement::OnBackgroundChanged(const Brush& background)
	{
		InvalidateBackground();
	}

	void UIElement::OnAttachedToLogicalTree()
	{
		Layoutable::OnAttachedToLogicalTree();

		auto parent = static_cast<Renderable*>(GetStylingParent());
		SetDataContext(parent->GetDataContext(), ValueSource::Inheritance);
		SetCursor(parent->GetCursor(), ValueSource::Inheritance);

		ApplyBindings();
	}

	std::span<const Ref<UIElement>> UIElement::GetChildren() const
	{
		return {};
	}

	void UIElement::OnKeyUp(KeyEventArgs e)
	{
		KeyUp.Invoke(*this, e);
	}

	void UIElement::OnKeyDown(KeyEventArgs e)
	{
		KeyDown.Invoke(*this, e);
	}

	void UIElement::OnMouseMove(MouseMoveEventArgs e)
	{
		bool wasMouseOver = IsMouseOver();
		bool isMouseOver = IsVisible() && IsPointInRect(e.X, e.Y, GetBounds());

		if(isMouseOver)
		{
			if(!wasMouseOver)
			{
				OnMouseEnter(e);		
			}

			MouseMove.Invoke(*this, e);			
		}
		else if (wasMouseOver)
		{
			OnMouseLeave(e);
		}	

		if(_parent)
		{
			_parent->OnMouseMove(e);
		}
	}

	void UIElement::OnMouseDown(MouseButtonEventArgs e)
	{
		if(e.Button == MouseButton::Left)
		{
			PseudoClasses.Set(OnPressed);
		}

		MouseDown.Invoke(*this, e);

		if(_parent)
		{
			_parent->OnMouseDown(e);
		}
	}

	void UIElement::OnMouseUp(MouseButtonEventArgs e)
	{
		if(e.Button == MouseButton::Left)
		{
			PseudoClasses.Reset(OnPressed);
		}

		MouseUp.Invoke(*this, e);

		if(_parent)
		{
			_parent->OnMouseUp(e);
		}
	}

	void UIElement::OnMouseWheelChanged(MouseWheelEventArgs& e)
	{
		MouseWheel.Invoke(*this, e);
	}

	void UIElement::OnMouseEnter(MouseMoveEventArgs e)
	{
		//Logging::LogInfo("OnMouseEnter: {}", Name);
		PseudoClasses.Set(OnHover);
		MouseEnter.Invoke(*this, e);
	}

	void UIElement::OnMouseLeave(MouseMoveEventArgs e)
	{
		//Logging::LogInfo("OnMouseLeave: {}", Name);
		MouseLeave.Invoke(*this, e);
		PseudoClasses.Reset(OnHover);
	}

	RenderFragment UIElement::CreateBackgroundFragment(const Brush& background)
	{
		if(std::holds_alternative<Color>(background))
		{
			Color color = std::get<Color>(background);
			
			if(_cornersRadius > 0.0f)
			{
				return RenderFragments::RoundedRectangle(color, _cornersRadius);
			}
			else
			{
				return RenderFragments::Rectangle(color);
			}
		}
		else
		{
			auto& source = std::get<ImageSource>(background);
			auto texture = GetVisualRoot()->GetTextureFactory().Create(source, false);

			if(_cornersRadius > 0.0f)
			{
				return RenderFragments::RoundedImage(texture, _cornersRadius);
			}
			else
			{
				return RenderFragments::Image(texture);
			}
		}
	}

	void UIElement::InvalidateBackground()
	{
		_backgroundFragment = nullptr;
	}

	Ref<UIElement> UIElementDataTemplate::Build(const Ref<INotifyPropertyChanged>& data)
	{
		return data.As<UIElement>();
	}

	bool UIElementDataTemplate::Match(const Ref<INotifyPropertyChanged>& data) const
	{
		return data.Is<UIElement>();
	}
}