#include "UIElement.h"
#include "../Window.h"
#include "../Layout/LayoutHelper.h"

#include <cassert>
#include <ranges>
#include "../Base/Logging.h"
#include <queue>

namespace Sgl
{
	namespace
	{
		struct BackgroundVisitor
		{
			ITextureFactory* factory;

			RenderFragment operator()(Color color) const
			{
				return [color](RenderContext context, const FRect& rect)
				{
					context.DrawRectangleFill(rect, color);
				};
			}

			RenderFragment operator()(const ImageSource& imageSource) const
			{
				Texture texture = factory->Create(imageSource, false);
				return [texture](RenderContext context, const FRect& rect)
				{
					context.DrawTexture(texture, &rect, nullptr);
				};
			}
		};

		struct RoundedBackgroundVisitor
		{
			float cornersRadius;
			ITextureFactory* factory;

			RenderFragment operator()(Color color) const
			{
				float radius = cornersRadius;
				return [color, radius](RenderContext context, const FRect& rect)
				{
					context.DrawRectangleFill(rect, radius, color);
				};
			}

			RenderFragment operator()(const ImageSource& imageSource) const
			{
				float radius = cornersRadius;
				Texture texture = factory->Create(imageSource, false);
				return [texture, radius](RenderContext context, const FRect& rect)
				{
					context.DrawRectangleFill(rect, radius, texture);
				};
			}
		};
	}

	void UIElement::SetTag(const Any& value)
	{
		SetProperty(TagProperty, _tag, value, _tagSource);
	}

	void UIElement::SetCornersRadius(float value)
	{
		SetProperty(CornersRadiusProperty, _cornersRadius, value, _cornersRadiusSource);
	}

	void UIElement::SetIsFocusable(bool value)
	{
		SetProperty(IsFocusableProperty, _focusable, value, _focusableSource);
	}

	void UIElement::SetClipToBounds(bool value)
	{
		SetProperty(ClipToBoundsProperty, _clipToBounds, value, _clipToBoundsSource);
	}

	Ref<UIElement> UIElement::HitTest(FPoint point)
	{
		if(!IsVisible() || (_clipToBounds && !IsPointInRect(point.x, point.y, GetBounds())))
		{
			return nullptr;
		}

		Ref<UIElement> hit;

		for(auto& child : GetChildren() | std::views::reverse)
		{
			hit = child->HitTest(point);

			if(hit)
			{
				return hit;
			}
		}

		return IsPointInRect(point.x, point.y, GetBounds()) ? Ref(this) : nullptr;
	}

	bool UIElement::Focus()
	{
		if(auto window = static_cast<Window*>(GetVisualRoot()))
		{
			return window->GetFocusManager().SetFocus(Ref(this));
		}

		return false;
	}

	void UIElement::Render(RenderContext& context)
	{
		if(!IsVisible())
		{
			return;
		}

		if(_clipToBounds)
		{
			context.PushClip(ToRect(GetBounds()));
		}

		if(_renderBackground)
		{
			if(!_backgroundFragment)
			{
				_backgroundFragment = CreateBackgroundFragment(GetBackground());
			}

			_backgroundFragment(context, GetBounds());
		}

		OnRender(context);

		for(auto& child : _children)
		{
			child->Render(context);
		}

		if(_clipToBounds)
		{
			context.PopClip();
		}
	}

	void UIElement::ApplyStyling()
	{
		ApplyStyle();

		for(auto& child : _children)
		{
			child->ApplyStyling();
		}
	}

	Ref<UIElement> UIElement::FindChild(const std::string& name)
	{
		std::queue<UIElement*> queue;
		
		for(auto& child : GetChildren())
		{
			queue.push(child.Get());
		}

		while(!queue.empty())
		{
			auto current = queue.front();
			queue.pop();

			if(current->GetName() == name)
			{
				return Ref(current);
			}

			for(auto& child : current->GetChildren())
			{
				queue.push(child.Get());
			}
		}

		return nullptr;
	}

	void UIElement::OnPropertyChanged(PropertyBase& property)
	{
		Layoutable::OnPropertyChanged(property);

		if(property == CornersRadiusProperty)
		{
			InvalidateRender();
			InvalidateBackground();
		}
	}

	void UIElement::SetParent(Styleable* parent)
	{
		Layoutable::SetParent(parent);
		_parent = dynamic_cast<UIElement*>(parent);
	}

	void UIElement::OnCursorChanged(Cursor cursor)
	{
		for(auto& child : _children)
		{
			ValueSourceScope scope(child, ValueSource::Inheritance);
			child->SetCursor(cursor);
		}

		if(IsMouseOver())
		{
			SDL_SetCursor(cursor);
		}
	}

	void UIElement::OnBackgroundChanged(const Brush& background)
	{
		_renderBackground = !IsTransparent(background);
		InvalidateBackground();
	}

	void UIElement::OnDataContextChanged(const Ref<ObservableObject>& dataContext)
	{
		for(auto& child : _children)
		{
			ValueSourceScope scope(child, ValueSource::Inheritance);
			child->SetDataContext(dataContext);
		}
	}

	void UIElement::OnAttachedToLogicalTree()
	{
		Layoutable::OnAttachedToLogicalTree();

		{
			ValueSourceScope scope(*this, ValueSource::Inheritance);
			auto parent = static_cast<Renderable*>(GetStylingParent());
			SetDataContext(parent->GetDataContext());
			SetCursor(parent->GetCursor());
		}
		
		ApplyBindings();

		for(auto& child : _children)
		{
			child->OnAttachedToLogicalTree();
		}
	}

	void UIElement::OnDetachedFromLogicalTree()
	{
		Layoutable::OnDetachedFromLogicalTree();

		_backgroundFragment = nullptr;

		for(auto& child : _children)
		{
			child->OnDetachedFromLogicalTree();
		}
	}

	FSize UIElement::MeasureContent(FSize availableSize)
	{
		FSize size {};

		for(auto& child : _children)
		{
			child->Measure(availableSize);
			size += child->GetDesiredSize();
		}

		return size;
	}

	void UIElement::ArrangeContent(FRect rect)
	{
		for(auto& child : _children)
		{
			child->Arrange(rect);
		}
	}

	void UIElement::OnKeyDown(KeyEventArgs& e)
	{
		KeyDown.Invoke(*this, e);
	}

	void UIElement::OnKeyUp(KeyEventArgs& e)
	{
		KeyUp.Invoke(*this, e);
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
	}

	void UIElement::OnMouseDown(MouseClickEventArgs& e)
	{
		if(e.Button == MouseButton::Left)
		{
			States.Set(OnPressed);
		}

		MouseDown.Invoke(*this, e);
	}

	void UIElement::OnMouseUp(MouseClickEventArgs& e)
	{
		if(e.Button == MouseButton::Left)
		{
			States.Reset(OnPressed);
		}

		MouseUp.Invoke(*this, e);
	}

	void UIElement::OnMouseWheelChanged(MouseWheelEventArgs& e)
	{
		MouseWheel.Invoke(*this, e);
	}

	void UIElement::OnMouseEnter(MouseMoveEventArgs e)
	{
		//Logging::LogInfo("OnMouseEnter: {}", Name);
		States.Set(OnHover);
		MouseEnter.Invoke(*this, e);
	}

	void UIElement::OnMouseLeave(MouseMoveEventArgs e)
	{
		//Logging::LogInfo("OnMouseLeave: {}", Name);
		MouseLeave.Invoke(*this, e);
		States.Reset(OnHover);
	}

	void UIElement::OnGotFocus(EventArgs e)
	{
		//Logging::LogInfo("OnGotFocus: {}", Name);
		States.Set(OnFocus);
		GotFocus.Invoke(*this, e);
	}

	void UIElement::OnLostFocus(EventArgs e)
	{
		//Logging::LogInfo("OnLostFocus: {}", Name);
		LostFocus.Invoke(*this, e);
		States.Reset(OnFocus);
	}

	void UIElement::AddChild(const Ref<UIElement>& child)
	{
		_children.push_back(child);
		child->SetParent(this);

		if(IsAttachedToLogicalTree())
		{
			child->OnAttachedToLogicalTree();
		}
	}

	void UIElement::RemoveChild(const Ref<UIElement>& child)
	{
		std::erase(_children, child);
		child->SetParent(nullptr);

		if(IsAttachedToLogicalTree())
		{
			child->OnDetachedFromLogicalTree();
		}
	}

	void UIElement::ClearChildren()
	{
		if(IsAttachedToLogicalTree())
		{
			for(auto& child : _children)
			{
				child->SetParent(nullptr);
				child->OnDetachedFromLogicalTree();
			}
		}
		else
		{
			for(auto& child : _children)
			{
				child->SetParent(nullptr);
			}
		}

		_children.clear();
	}

	RenderFragment UIElement::CreateBackgroundFragment(const Brush& background)
	{
		auto& textureFactory = GetVisualRoot()->GetTextureFactory();
		return _cornersRadius > 0.0f
			? std::visit(RoundedBackgroundVisitor(_cornersRadius, &textureFactory), background)
			: std::visit(BackgroundVisitor(&textureFactory), background);
	}

	void UIElement::InvalidateBackground()
	{
		_backgroundFragment = nullptr;
	}

	Ref<UIElement> UIElementDataTemplate::Build(const Ref<ObservableObject>& data)
	{
		return data.As<UIElement>();
	}

	bool UIElementDataTemplate::Match(const Ref<ObservableObject>& data) const
	{
		return data.Is<UIElement>();
	}

	void UIElementStyle::OnState(StyleContext& context, const VisualStateSet& states)
	{
		if(states.Has(UIElement::OnHover))
		{
			OnHover(context);
		}

		if(states.Has(UIElement::OnFocus))
		{
			OnFocus(context);
		}

		if(states.Has(UIElement::OnPressed))
		{
			OnPressed(context);
		}
	}
}