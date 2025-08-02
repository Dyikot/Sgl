#pragma once

#include "../Base/Any.h"
#include "../Base/Observable/Event.h"
#include "../Base/Collections/ResourcesMap.h"
#include "../Input/Cursor.h"
#include "../Input/MouseAndKeyEventArgs.h"
#include "../Render/IRenderable.h"
#include "Layoutable.h"
#include <type_traits>

namespace Sgl
{
	class Scene;

	class UIElement: public Layoutable, public IRenderable
	{
	private:
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

		ResourcesMap Resources;
		Shared<void> DataContext;
	protected:
		bool _isMouseOver;
	private:
		std::reference_wrapper<const Cursor> _cursor;
		Shared<Texture> _backgroundTexture;
		Color _backgroundColor;
		Any _tag;
		Shared<UIElement> _toolTip;
		size_t _zIndex;
	public:
		UIElement();
		UIElement(const UIElement& other);
		UIElement(UIElement&& other) noexcept;
		virtual ~UIElement() = default;
		
		void SetCursor(const Cursor& value)
		{ 
			SetProperty<UIElement, std::reference_wrapper<const Cursor>, const Cursor&>(
				CursorProperty, _cursor, value);		
		}

		const Cursor& GetCursor() const
		{ 
			return _cursor;
		}

		void SetBackgroundTexture(Shared<Texture> value) 
		{ 
			SetProperty(BackgroundTextureProperty, _backgroundTexture, value);
		}

		Shared<Texture> GetBackgroundTexture() const
		{ 
			return _backgroundTexture; 
		}

		void SetBackgroundColor(Color value) 
		{ 
			SetProperty(BackgroundColorProperty, _backgroundColor, value);
		}

		Color GetBackgroundColor() const 
		{ 
			return _backgroundColor;
		}

		void SetTag(const Any& value) 
		{ 
			SetProperty<UIElement, Any, const Any&>(TagProperty, _tag, value);
		}

		const Any& GetTag() const 
		{ 
			return _tag;
		}

		void SetToolTip(Shared<UIElement> value) 
		{ 
			SetProperty(ToolTipProperty, _toolTip, value);
		}

		Shared<UIElement> GetToolTip() const
		{ 
			return _toolTip;
		}

		void SetZIndex(size_t value)
		{ 
			SetProperty(ZIndexProperty, _zIndex, value);
		}

		size_t GetZIndex() const 
		{ 
			return _zIndex;
		}

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
			Cursor::Set(_cursor);
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
	public:
		static inline BindableProperty<UIElement, std::reference_wrapper<const Cursor>, const Cursor&> CursorProperty =
			BindableProperty<UIElement, std::reference_wrapper<const Cursor>, const Cursor&>(&SetCursor, Cursors::Arrow);

		static inline BindableProperty<UIElement, Shared<Texture>> BackgroundTextureProperty =
			BindableProperty<UIElement, Shared<Texture>>(&SetBackgroundTexture);

		static inline BindableProperty<UIElement, Color> BackgroundColorProperty =
			BindableProperty<UIElement, Color>(&SetBackgroundColor, Colors::Transparent);

		static inline BindableProperty<UIElement, Any, const Any&> TagProperty =
			BindableProperty<UIElement, Any, const Any&>(&SetTag);

		static inline BindableProperty<UIElement, Shared<UIElement>> ToolTipProperty =
			BindableProperty<UIElement, Shared<UIElement>>(&SetToolTip);

		static inline BindableProperty<UIElement, size_t> ZIndexProperty =
			BindableProperty<UIElement, size_t>(&SetZIndex);
	};

	struct UIElementComparer
	{
		bool operator()(const UIElement& left, const UIElement& right) const
		{
			return left.GetZIndex() < right.GetZIndex();
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