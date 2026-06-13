#include "UIElement.h"
#include "../Application.h"

namespace Sgl
{
	UIElement::UIElement(UIElement&& other) noexcept:
		Layoutable(std::move(other)),
		_backgroundRenderer(std::move(other._backgroundRenderer)),
		_tag(std::move(other._tag)),
		_isCornersRounded(other._isCornersRounded),
		_tagSource(other._tagSource),
		_isCornersRoundedSource(other._isCornersRoundedSource)
	{}

	void UIElement::SetTag(const Any& value, ValueSource source)
	{
		SetProperty(TagProperty, _tag, value, _tagSource, source);
	}

	void UIElement::SetIsCornersRounded(bool value, ValueSource source)
	{
		if(SetProperty(IsCornersRoundedProperty, _isCornersRounded, value, _isCornersRoundedSource, source))
		{
			InvalidateRender();
			UpdateBackgroundRenderer(GetBackground());
		}
	}

	void UIElement::Render(RenderContext context)
	{
		Renderable::Render(context);
		_backgroundRenderer(context, GetBounds());
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
		UpdateBackgroundRenderer(background);
	}

	void UIElement::OnAttachedToLogicalTree()
	{
		Layoutable::OnAttachedToLogicalTree();

		auto& parent = static_cast<Renderable&>(*GetStylingParent());
		SetDataContext(parent.GetDataContext(), ValueSource::Inheritance);
		SetCursor(parent.GetCursor(), ValueSource::Inheritance);

		if(!_backgroundRenderer)
		{
			UpdateBackgroundRenderer(GetBackground());
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

	static constexpr size_t ColorBg   = 0;
	static constexpr size_t TextureBg = 1;
	static constexpr size_t RoundedColorBg   =   ColorBg | 2;
	static constexpr size_t RoundedTextureBg = TextureBg | 2;

	void UIElement::UpdateBackgroundRenderer(const Brush& background)
	{
		if(!IsAttachedToLogicalTree())
		{
			return;
		}

		auto states = background.index() | (static_cast<size_t>(_isCornersRounded) << 1);

		switch(states)
		{
			case ColorBg:
			{
				auto color = std::get<Color>(background);
				_backgroundRenderer = [color](RenderContext context, const FRect& rect)
				{
					context.DrawRectangleFill(rect, color);
				};

				break;
			}

			case TextureBg:
			{
				auto& source = std::get<ImageSource>(background);
				auto texture = GetVisualRoot()->GetTextureFactory().Create(source, false);
				_backgroundRenderer = [texture](RenderContext context, const FRect& rect)
				{
					context.DrawTexture(texture, &rect, nullptr);
				};

				break;
			}

			case RoundedColorBg:
			{
				auto color = std::get<Color>(background);
				auto circleFill = GetVisualRoot()->GetTextureFactory().CreatePrimitive(0);
				_backgroundRenderer = [color, circleFill](RenderContext context, const FRect& rect) mutable
				{
					circleFill.SetColor(color);
					context.DrawTexture9Grid(circleFill, 16, 1, &rect, nullptr);
				};

				break;
			}

			case RoundedTextureBg:
			{
				auto& factory = GetVisualRoot()->GetTextureFactory();

				auto& source = std::get<ImageSource>(background);
				auto texture = factory.Create(source, false);
				auto circleFill = factory.CreatePrimitive(0);
				_backgroundRenderer = [texture, circleFill, cache = Texture()]
					(RenderContext context, const FRect& rect) mutable
				{
					if(!cache || cache.GetWidth() != rect.w || cache.GetHeight() != rect.h)
					{
						cache = Texture(context.GetRenderer(), 
										 Size(rect.w, rect.h),
										 TextureAccess::Target);

						context.SetTarget(cache);
						{
							circleFill.SetColor(Colors::White);
							context.DrawTexture9Grid(circleFill, 16, 1, nullptr, nullptr);

							texture.SetBlendMode(SDL_BLENDMODE_MOD);
							context.DrawTexture(texture, nullptr, nullptr);
							texture.SetBlendMode(SDL_BLENDMODE_BLEND);
						}
						context.ResetTarget();
					}

					context.DrawTexture(cache, &rect, nullptr);
				};

				break;
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