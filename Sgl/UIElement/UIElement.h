#pragma once

#include <type_traits>

#include "../Base/Any.h"
#include "../Base/Event.h"
#include "../Data/ObservableObject.h"
#include "../Data/IDataTemplate.h"
#include "../Data/BindingMode.h"
#include "../Input/MouseAndKeyEventArgs.h"
#include "../Layout/Layoutable.h"

namespace Sgl
{
	class UIElement : public Layoutable
	{
	private:
		using UIElementEventHandler = EventHandler<UIElement>;
		using KeyEventHandler = EventHandler<UIElement, KeyEventArgs>;
		using MouseMoveEventHandler = EventHandler<UIElement, MouseMoveEventArgs>;
		using MouseButtonEventHandler = EventHandler<UIElement, MouseButtonEventArgs>;
		using MouseWheelEventHandler = EventHandler<UIElement, MouseWheelEventArgs&>;
	public:
		Event<KeyEventHandler> KeyUp;
		Event<KeyEventHandler> KeyDown;
		Event<MouseMoveEventHandler> MouseMove;
		Event<MouseMoveEventHandler> MouseEnter;
		Event<MouseMoveEventHandler> MouseLeave;
		Event<MouseButtonEventHandler> MouseUp;
		Event<MouseButtonEventHandler> MouseDown;
		Event<MouseWheelEventHandler> MouseWheel;		
	private:		
		Any _tag;		
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

		void SetToolTip(Ref<UIElement> value);
		Ref<UIElement> GetToolTip() const { return _toolTip; }

		void SetDataContext(Ref<ObservableObject> value);
		Ref<ObservableObject> GetDataContext() const { return _dataContext; }

		bool IsMouseOver() const { return _isMouseOver; }

		void Render(RenderContext context) override;

		template<typename TTarget, typename TTargetValue,
				 typename TSource, typename TSourceValue,
				 typename TConverter = std::identity>
		void Bind(ObservableProperty<TTarget, TTargetValue>& targetProperty,
				  ObservableProperty<TSource, TSourceValue>& sourceProperty,
				  BindingMode mode = BindingMode::OneWay,
				  TConverter converter = {})
		{
			if(_dataContext)
			{
				auto& target = static_cast<TTarget&>(*this);
				auto& source = _dataContext.GetValueAs<TSource>();

				switch(mode)
				{
					case BindingMode::OneWay:
					{
						targetProperty.Set(target, converter(sourceProperty.Get(source)));
						source.SetObserver(sourceProperty, target, targetProperty, converter);
						break;
					}

					case BindingMode::OneWayToSource:
					{
						sourceProperty.Set(source, converter(targetProperty.Get(target)));
						SetObserver(targetProperty, source, sourceProperty, converter);
						break;
					}

					case BindingMode::TwoWay:
					{
						targetProperty.Set(target, converter(sourceProperty.Get(source)));
						sourceProperty.Set(source, converter(targetProperty.Get(target)));
						SetObserver(targetProperty, source, sourceProperty, converter);
						break;
					}

					case BindingMode::OneTime:
					{
						targetProperty.Set(target, converter(sourceProperty.Get(source)));
						break;
					}
				}
			}
			else
			{
				throw std::exception("Data context is null");
			}
		}
		
		template<typename TTarget, typename TSource,
				 typename TValue, typename TConverter = std::identity>
		void Bind(ObservableProperty<TTarget, TValue>& targetProperty,
				  TValue TSource::* sourceField,
				  TConverter converter = {})
		{
			if(_dataContext)
			{
				auto& target = static_cast<TTarget&>(*this);
				auto& source = _dataContext.GetValueAs<TSource>();
				targetProperty.Set(target, converter(source.*sourceField));
			}
			else
			{
				throw std::exception("Data context is null");
			}
		}

		template<typename TTarget, typename TSource, 
				 typename TValue, typename TConverter = std::identity>
		void Bind(ObservableProperty<TTarget, TValue>& targetProperty,
				  TValue (TSource::* sourceMethod)(),
				  TConverter converter = {})
		{
			if(_dataContext)
			{
				auto& target = static_cast<TTarget&>(*this);
				auto& source = _dataContext.GetValueAs<TSource>();
				targetProperty.Set(target, converter((source.*sourceMethod)()));
			}
			else
			{
				throw std::exception("Data context is null");
			}
		}
	protected:
		void RenderBackground(RenderContext context);
		void OnCursorChanged(const Cursor& cursor) override;
		virtual void OnKeyUp(KeyEventArgs e);
		virtual void OnKeyDown(KeyEventArgs e);
		virtual void OnMouseMove(MouseMoveEventArgs e);
		virtual void OnMouseDown(MouseButtonEventArgs e);
		virtual void OnMouseUp(MouseButtonEventArgs e);
		virtual void OnMouseWheelChanged(MouseWheelEventArgs& e);
		virtual void OnMouseEnter(MouseMoveEventArgs e);
		virtual void OnMouseLeave(MouseMoveEventArgs e);
		void OnAttachedToLogicalTree() override;
		void OnDetachedFromLogicalTree() override;
	public:
		static inline ObservableProperty TagProperty { &SetTag, &GetTag };
		static inline ObservableProperty ToolTipProperty { &SetToolTip, &GetToolTip };
		static inline ObservableProperty DataContextProperty { &SetDataContext, &GetDataContext };

		friend class Panel;
		friend class Window;
		friend class ContentUIElement;
		friend class UIElementsCollection;
	};

	class UIElementDataTemplate : public IDataTemplate
	{
	public:
		Ref<UIElement> Build(const Ref<ObservableObject>& data) override;
		bool Match(const Ref<ObservableObject>& data) const override;
	};
}