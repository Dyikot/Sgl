#pragma once

#include "../Base/Any.h"
#include "../Base/Observable/Event.h"
#include "../Base/Collections/ResourcesMap.h"
#include "../Input/Cursor.h"
#include "../Input/MouseAndKeyEventArgs.h"
#include "../Render/IRenderable.h"
#include "Layoutable.h"

namespace Sgl
{
	class Scene;

	class UIElement: public Layoutable, public IRenderable
	{
	private:
		using ColorProperty = BindableProperty<Color>;
		using CursorProperty = BindableProperty<std::reference_wrapper<const Cursor>, const Cursor&>;
		using TextureProperty = BindableProperty<Shared<Texture>>;
		using TagProperty = BindableProperty<Any, const Any&>;
		using ToolTipProperty = BindableProperty<Shared<UIElement>>;
		using ZIndexProperty = BindableProperty<size_t>;

		using KeyEventHandler = EventHandler<UIElement, KeyEventArgs>;
		using MouseEventHandler = EventHandler<UIElement, MouseEventArgs>;
		using MouseButtonEventHandler = EventHandler<UIElement, MouseButtonEventArgs>;
		using MouseWheelEventHandler = EventHandler<UIElement, MouseWheelEventArgs>;
	public:
		Event<KeyEventHandler> KeyUp;
		Event<KeyEventHandler> KeyDown;
		Event<MouseEventHandler> MouseMove;
		Event<MouseEventHandler> MouseEnter;
		Event<MouseEventHandler> MouseLeave;
		Event<MouseButtonEventHandler> MouseUp;
		Event<MouseButtonEventHandler> MouseDown;
		Event<MouseWheelEventHandler> MouseWheel;

		CursorProperty Cursor;
		TextureProperty BackgroundTexture;
		ColorProperty BackgroundColor;
		TagProperty Tag;
		ToolTipProperty ToolTip;
		ZIndexProperty ZIndex;

		ResourcesMap Resources;
		Any DataContext;
	protected:
		bool _isMouseOver;
	public:
		UIElement();
		UIElement(const UIElement& other);
		UIElement(UIElement&& other) noexcept;
		virtual ~UIElement() = default;
		
		Scene& GetScene();
		bool IsMouseOver() const { return _isMouseOver; }

		void Render(RenderContext context) const override;
	protected:
		virtual void OnKeyUp(const KeyEventArgs& e)
		{
			KeyUp.TryInvoke(*this, e);
		}

		virtual void OnKeyDown(const KeyEventArgs& e)
		{
			KeyDown.TryInvoke(*this, e);
		}

		virtual void OnMouseMove(const MouseEventArgs& e)
		{
			MouseMove.TryInvoke(*this, e);
		}

		virtual void OnMouseDown(const MouseButtonEventArgs& e)
		{
			MouseDown.TryInvoke(*this, e);
		}

		virtual void OnMouseUp(const MouseButtonEventArgs& e)
		{ 
			MouseUp.TryInvoke(*this, e);
		}

		virtual void OnMouseWheelChanged(const MouseWheelEventArgs& e)
		{ 
			MouseWheel.TryInvoke(*this, e);
		}

		virtual void OnMouseEnter(const MouseEventArgs& e)
		{
			_isMouseOver = true;
			Cursor::Set(Cursor);
			MouseEnter.TryInvoke(*this, e);
		}

		virtual void OnMouseLeave(const MouseEventArgs& e)
		{
			MouseLeave.TryInvoke(*this, e);
			_isMouseOver = false;
		}
		
		friend class Scene;
		friend class Panel;
		friend class ContentUIElement;
		friend class UIElementsCollection;
	};

	struct UIElementComparer
	{
		bool operator()(const UIElement& left, const UIElement& right) const
		{
			return left.ZIndex < right.ZIndex;
		}
	};

	template<typename T, typename TConfigurer>
	Shared<T> Create(TConfigurer&& configurer)
	{
		auto element = NewShared<T>();
		configurer(*element);
		return element;
	}

	template<typename T, typename TConfigurer>
	Ref<T> CreateRef(TConfigurer&& configurer)
	{
		auto element = NewRef<T>();
		configurer(*element);
		return element;
	}
}