#pragma once

#include <type_traits>
#include "../Base/Any.h"
#include "../Base/Event.h"
#include"../Data/BindingMode.h"
#include "../Input/MouseAndKeyEventArgs.h"
#include "Layoutable.h"

namespace Sgl
{
	class Scene;

	class UIElement: public Layoutable
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
	private:		
		Any _tag;
		Ref<UIElement> _toolTip;
		size_t _zIndex = 0;
		Ref<ObservableObject> _dataContext;

		bool _isMouseOver = false;
	public:
		UIElement() = default;
		UIElement(const UIElement& other);
		UIElement(UIElement&& other) noexcept;
		virtual ~UIElement() = default;
		
		void SetTag(const Any& value);
		const Any& GetTag() const { return _tag; }

		void SetToolTip(Ref<UIElement> value);
		Ref<UIElement> GetToolTip() const { return _toolTip; }

		void SetZIndex(size_t value);
		size_t GetZIndex() const { return _zIndex; }

		void SetDataContext(Ref<ObservableObject> value);
		Ref<ObservableObject> GetDataContext() const { return _dataContext; }

		bool IsMouseOver() const { return _isMouseOver; }

		void Render(RenderContext context) override;

		template<typename TObservable, typename TObserver, typename TMember>
		void Bind(ObservableProperty<TObservable, TMember>& observableProperty,
				  ObservableProperty<TObserver, TMember>& observerProperty,
				  BindingMode mode = BindingMode::OneWay)
		{
			if(_dataContext)
			{
				auto& observer = _dataContext.GetValueAs<TObserver>();

				switch(mode)
				{
					case Sgl::BindingMode::OneWay:
						SetObserver(observableProperty, observer, observerProperty);
						break;

					case Sgl::BindingMode::OneWayToSource:
						observer.SetObserver(observerProperty, *this, observableProperty);
						break;

					case Sgl::BindingMode::TwoWay:
						SetObserver(observableProperty, observer, observerProperty);
						observer.SetObserver(observerProperty, *this, observableProperty);
						break;
				}				
			}
			else
			{
				throw std::exception("Data context is null");
			}
		}
	protected:
		void RenderBackground(RenderContext context);
		void OnCursorChanged(const Cursor& cursor) override;
		virtual void OnKeyUp(const KeyEventArgs& e);
		virtual void OnKeyDown(const KeyEventArgs& e);
		virtual void OnMouseMove(const MouseEventArgs& e);
		virtual void OnMouseDown(const MouseButtonEventArgs& e);
		virtual void OnMouseUp(const MouseButtonEventArgs& e);
		virtual void OnMouseWheelChanged(const MouseWheelEventArgs& e);
		virtual void OnMouseEnter(const MouseEventArgs& e);
		virtual void OnMouseLeave(const MouseEventArgs& e);
	public:
		static inline ObservableProperty<UIElement, const Any&> TagProperty =
			ObservableProperty<UIElement, const Any&>(&SetTag, &GetTag);

		static inline ObservableProperty<UIElement, Ref<UIElement>> ToolTipProperty =
			ObservableProperty<UIElement, Ref<UIElement>>(&SetToolTip, &GetToolTip);

		static inline ObservableProperty<UIElement, size_t> ZIndexProperty =
			ObservableProperty<UIElement, size_t>(&SetZIndex, &GetZIndex);

		static inline ObservableProperty<UIElement, Ref<ObservableObject>> DataContextProperty =
			ObservableProperty<UIElement, Ref<ObservableObject>>(&SetDataContext, &GetDataContext);

		friend class Scene;
		friend class Panel;
		friend class ContentUIElement;
		friend class UIElementsCollection;
	};

	struct UIElementComparer
	{
		bool operator()(const Ref<UIElement>& left, const Ref<UIElement>& right) const
		{
			return left->GetZIndex() < right->GetZIndex();
		}
	};

	struct UIElementDataTemplate
	{
		Ref<UIElement> operator()(const Any& data) const
		{
			return data.As<Ref<UIElement>>();
		}
	};
}