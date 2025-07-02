#pragma once

#include "../Data/Object.h"
#include "../Style/Layout.h"
#include "../Style/StyleableElement.h"
#include "../Style/StyleableProperty.h"
#include "../Events/Event.h"
#include "../Events/MouseAndKeyArgs.h"
#include "../Render/Renderer.h"
#include "../Render/IRenderable.h"
#include "../Render/Cursor.h"
#include "ToolTip.h"

namespace Sgl
{
	class UIElement: public StyleableElement, public IRenderable
	{
	public:
		class WidthProperty: public StylyableProperty<float>
		{
		private:
			UIElement& _owner;
		public:
			WidthProperty(UIElement& owner):
				StylyableProperty<float>(0),
				_owner(owner)
			{}

			void Set(float value) override
			{
				_value = value;
				_owner._actualWidth = value - _owner.Margin->Right;
				_owner.OnSizeChanged();
			}

			using StylyableProperty<float>::operator=;
		};

		class HeightProperty: public StylyableProperty<float>
		{
		private:
			UIElement& _owner;
		public:
			HeightProperty(UIElement& owner):
				StylyableProperty<float>(0),
				_owner(owner)
			{}

			void Set(float value) override
			{
				_value = value;
				_owner._actualHeight = value - _owner.Margin->Bottom;
				_owner.OnSizeChanged();
			}

			using StylyableProperty<float>::operator=;
		};

		using FloatProperty = StylyableProperty<float>;
		using BoolProperty = StylyableProperty<bool>;
		using ZIndexProperty = StylyableProperty<size_t>;
		using ThicknessProperty = StylyableProperty<Thickness>;
		using VerticalAlignmentProperty = StylyableProperty<VerticalAlignment>;
		using HorizontalAlignmentProperty = StylyableProperty<HorizontalAlignment>;
		using ColorProperty = StylyableProperty<Color>;
		using CursorProperty = StylyableProperty<std::reference_wrapper<const Cursor>, const Cursor&>;
		using TextureProperty = StylyableProperty<shared_ptr<Texture>>;

		using KeyEventHandler = EventHandler<UIElement, KeyEventArgs>;
		using MouseEventHandler = EventHandler<UIElement, MouseEventArgs>;
		using MouseButtonEventHandler = EventHandler<UIElement, MouseButtonEventArgs>;
		using MouseWheelEventHandler = EventHandler<UIElement, MouseWheelEventArgs>;
		using SizeChangedEventHandler = EventHandler<UIElement, EventArgs>;

		Event<KeyEventHandler> KeyUp;
		Event<KeyEventHandler> KeyDown;
		Event<MouseEventHandler> MouseMove;
		Event<MouseEventHandler> MouseEnter;
		Event<MouseEventHandler> MouseLeave;
		Event<MouseButtonEventHandler> MouseUp;
		Event<MouseButtonEventHandler> MouseDown;
		Event<MouseWheelEventHandler> MouseWheel;
		Event<SizeChangedEventHandler> SizeChanged;

		WidthProperty Width;
		HeightProperty Height;
		FloatProperty MinWidth;
		FloatProperty MinHeight;
		FloatProperty MaxWidth;
		FloatProperty MaxHeight;
		BoolProperty IsVisible;
		ZIndexProperty ZIndex;
		ThicknessProperty Margin;
		CursorProperty Cursor;
		TextureProperty Texture;
		ColorProperty BackgroundColor;
		VerticalAlignmentProperty VerticalAlignment;
		HorizontalAlignmentProperty HorizontalAlignment;
	private:
		float _actualWidth;
		float _actualHeight;
		FPoint _position;
		FPoint _actualPosition;
	public:
		UIElement();
		UIElement(const UIElement& other);
		UIElement(UIElement&& other) noexcept;
		virtual ~UIElement() = default;

		float GetActualWidth() const { return _actualWidth; }
		float GetActualHeight() const { return _actualHeight; }

		void SetTooltip(const ToolTip& tooltip);

		void OnRender(RenderContext context) const override;
	protected:
		FPoint GetPosition() const { return _position; }
		FPoint GetActualPosition() const { return _actualPosition; }

		virtual void OnKeyUp(const KeyEventArgs& e) { KeyUp.TryRaise(*this, e); }
		virtual void OnKeyDown(const KeyEventArgs& e) { KeyDown.TryRaise(*this, e); }
		virtual void OnMouseMove(const MouseEventArgs& e) { MouseMove.TryRaise(*this, e); }
		virtual void OnMouseDown(const MouseButtonEventArgs& e) { MouseDown.TryRaise(*this, e); }
		virtual void OnMouseUp(const MouseButtonEventArgs& e) { MouseUp.TryRaise(*this, e); }
		virtual void OnMouseWheel(const MouseWheelEventArgs& e) { MouseWheel.TryRaise(*this, e); }
		virtual void OnMouseEnter(const MouseEventArgs& e) { MouseEnter.TryRaise(*this, e); }
		virtual void OnMouseLeave(const MouseEventArgs& e) { MouseLeave.TryRaise(*this, e); }
		virtual void OnSizeChanged() { SizeChanged.TryRaise(*this, EmptyEventArgs); }
	private:
		void SetPosition(FPoint value)
		{
			_position = value;
			_actualPosition = FPoint
			{
				.x = static_cast<float>(value.x + Margin->Left),
				.y = static_cast<float>(value.y + Margin->Top)
			};
		}

		friend class Layout;
	};

	struct UIElementComparer
	{
		bool operator()(const shared_ptr<UIElement>& left, const shared_ptr<UIElement>& right) const
		{
			return left->ZIndex < right->ZIndex;
		}
	};
}