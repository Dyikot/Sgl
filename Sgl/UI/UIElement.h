#pragma once

#include <type_traits>
#include "../Base/Any.h"
#include "../Base/Event.h"
#include"../Data/BindingMode.h"
#include "../Data/DataTemplate.h"
#include "../Input/MouseAndKeyEventArgs.h"
#include "Layoutable.h"

namespace Sgl
{
	class Scene;

	class UIElement : public Layoutable, public IData
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
		std::string _name;
		Ref<UIElement> _toolTip;
		Ref<ObservableObject> _dataContext;

		bool _isMouseOver = false;
	public:
		UIElement() = default;
		UIElement(const UIElement& other);
		UIElement(UIElement&& other) noexcept;
		virtual ~UIElement() = default;
		
		void SetTag(const Any& value);
		const Any& GetTag() const { return _tag; }

		void SetName(const std::string& value);
		const std::string& GetName() const { return _name; }

		void SetToolTip(Ref<UIElement> value);
		Ref<UIElement> GetToolTip() const { return _toolTip; }

		void SetDataContext(Ref<ObservableObject> value);
		Ref<ObservableObject> GetDataContext() const { return _dataContext; }

		bool IsMouseOver() const { return _isMouseOver; }

		void Render(RenderContext context) override;
		DataTemplate GetDefaultDataTemplate() const override;

		template<typename TObservable, typename TObserver, typename TMember>
		void Bind(ObservableProperty<TObserver, TMember>& targetProperty,
				  ObservableProperty<TObservable, TMember>& sourceProperty,				  
				  BindingMode mode = BindingMode::OneWay)
		{
			if(_dataContext)
			{
				auto& source = _dataContext.GetValueAs<TObservable>();
				auto& target = static_cast<TObserver&>(*this);

				switch(mode)
				{
					case BindingMode::OneWay:
					{
						(target.*targetProperty.Setter)((source.*sourceProperty.Getter)());
						source.SetObserver(sourceProperty, target, targetProperty);
						break;
					}

					case BindingMode::OneWayToSource:
					{
						(source.*sourceProperty.Setter)((target.*targetProperty.Getter)());
						SetObserver(targetProperty, source, sourceProperty);
						break;
					}

					case BindingMode::TwoWay:
					{
						(target.*targetProperty.Setter)((source.*sourceProperty.Getter)());
						source.SetObserver(sourceProperty, target, targetProperty);
						SetObserver(targetProperty, source, sourceProperty);
						break;
					}
				}				
			}
			else
			{
				throw std::exception("Data context is null");
			}
		}

		template<typename TObserver, typename TObserverMember,
				 typename TObservable, typename TObservableMember,
				 typename TConverter>
		void Bind(ObservableProperty<TObserver, TObserverMember>& targetProperty,
				  ObservableProperty<TObservable, TObservableMember>& sourceProperty,
				  TConverter converter,
				  BindingMode mode = BindingMode::OneWay)
		{
			if(_dataContext)
			{
				auto& source = _dataContext.GetValueAs<TObservable>();
				auto& target = static_cast<TObserver&>(*this);

				switch(mode)
				{
					case BindingMode::OneWay:
					{
						(target.*targetProperty.Setter)(converter((source.*sourceProperty.Getter)()));
						source.SetObserver(sourceProperty, target, targetProperty, converter);
						break;
					}

					case BindingMode::OneWayToSource:
					{
						(source.*sourceProperty.Setter)(converter((target.*targetProperty.Getter)()));
						SetObserver(targetProperty, source, sourceProperty, converter);
						break;
					}

					case BindingMode::TwoWay:
					{
						(target.*targetProperty.Setter)(converter((source.*sourceProperty.Getter)()));
						source.SetObserver(sourceProperty, target, targetProperty, converter);
						SetObserver(targetProperty, source, sourceProperty, converter);
						break;
					}
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
		virtual void OnKeyUp(KeyEventArgs& e);
		virtual void OnKeyDown(KeyEventArgs& e);
		virtual void OnMouseMove(MouseEventArgs& e);
		virtual void OnMouseDown(MouseButtonEventArgs& e);
		virtual void OnMouseUp(MouseButtonEventArgs& e);
		virtual void OnMouseWheelChanged(MouseWheelEventArgs& e);
		virtual void OnMouseEnter(MouseEventArgs& e);
		virtual void OnMouseLeave(MouseEventArgs& e);
	public:
		static inline ObservableProperty TagProperty { &SetTag, &GetTag };
		static inline ObservableProperty ToolTipProperty { &SetToolTip, &GetToolTip };
		static inline ObservableProperty DataContextProperty { &SetDataContext, &GetDataContext };

		friend class Panel;
		friend class Window;
		friend class ContentUIElement;
		friend class UIElementsCollection;
	};	
}