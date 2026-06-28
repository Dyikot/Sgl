#include "UIElement.h"
#include "../Application.h"

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
			UpdateBackgroundFragment(GetBackground());
		}
	}

	void UIElement::Render(RenderContext context)
	{
		Renderable::Render(context);
		_backgroundFragment(context, GetBounds());
	}

	void UIElement::OnCursorChanged(Cursor cursor)
	{
		if(IsMouseOver())
		{
			_platformCursor.Set(cursor);
		}
	}

	void UIElement::OnBackgroundChanged(const Brush& background)
	{
		UpdateBackgroundFragment(background);
	}

	void UIElement::OnAttachedToLogicalTree()
	{
		Layoutable::OnAttachedToLogicalTree();

		auto& parent = static_cast<Renderable&>(*GetStylingParent());
		SetDataContext(parent.GetDataContext(), ValueSource::Inheritance);
		SetCursor(parent.GetCursor(), ValueSource::Inheritance);

		if(!_backgroundFragment)
		{
			UpdateBackgroundFragment(GetBackground());
		}

		ApplyBindings();
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
		MouseMove.Invoke(*this, e);
	}

	void UIElement::OnMouseDown(MouseButtonEventArgs e)
	{
		if(e.Button == MouseButton::Left)
		{
			PseudoClasses.Set(OnPressed);
		}

		MouseDown.Invoke(*this, e);
	}

	void UIElement::OnMouseUp(MouseButtonEventArgs e)
	{
		if(e.Button == MouseButton::Left)
		{
			PseudoClasses.Reset(OnPressed);
		}

		MouseUp.Invoke(*this, e);
	}

	void UIElement::OnMouseWheelChanged(MouseWheelEventArgs& e)
	{
		MouseWheel.Invoke(*this, e);
	}

	void UIElement::OnMouseEnter(MouseMoveEventArgs e)
	{
		_platformCursor.Set(GetCursor());
		PseudoClasses.Set(OnHover);
		MouseEnter.Invoke(*this, e);
	}

	void UIElement::OnMouseLeave(MouseMoveEventArgs e)
	{
		MouseLeave.Invoke(*this, e);
		PseudoClasses.Reset(OnHover);
	}

	void UIElement::UpdateBackgroundFragment(const Brush& background)
	{
		if(!IsAttachedToLogicalTree())
		{
			return;
		}

		if(std::holds_alternative<Color>(background))
		{
			Color color = std::get<Color>(background);
			
			if(_cornersRadius > 0.0f)
			{
				_backgroundFragment = RenderFragments::RoundedRectangle(color, _cornersRadius);
			}
			else
			{
				_backgroundFragment = RenderFragments::Rectangle(color);
			}
		}
		else
		{
			auto& source = std::get<ImageSource>(background);
			auto texture = GetVisualRoot()->GetTextureFactory().Create(source, false);

			if(_cornersRadius > 0.0f)
			{
				_backgroundFragment = RenderFragments::RoundedImage(texture, _cornersRadius);
			}
			else
			{
				_backgroundFragment = RenderFragments::Image(texture);
			}
		}
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