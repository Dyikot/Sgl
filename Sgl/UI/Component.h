#pragma once

#include <unordered_map>
#include <set>
#include "UIElement.h"
#include "../Appearance/Component.style.h"
#include "../Appearance/Cursor.h"
#include "../Binding/Binding.h"

namespace Sgl
{
	class Component: public UIElement
	{
	public:
		static inline const PropertyId WidthProperty = PropertyManager::Register<float>("Width");
		static inline const PropertyId HeightProperty = PropertyManager::Register<float>("Height");
		static inline const PropertyId MinWidthProperty = PropertyManager::Register<float>("MinWidth");
		static inline const PropertyId MinHeightProperty = PropertyManager::Register<float>("MinHeight");
		static inline const PropertyId MaxWidthProperty = PropertyManager::Register<float>("MaxWidth");
		static inline const PropertyId MaxHeightProperty = PropertyManager::Register<float>("MaxHeight");
		static inline const PropertyId ZIndexProperty = PropertyManager::Register<size_t>("ZIndex");
		static inline const PropertyId MarginProperty = PropertyManager::Register<Thikness>("Margin");
		static inline const PropertyId CursorProperty = PropertyManager::Register<const Cursor*>("Cursor");
		static inline const PropertyId ToolTipProperty = PropertyManager::Register<IVisual*>("ToolTip");
		static inline const PropertyId HorizontalAlignmentProperty = PropertyManager::Register<HorizontalAlignment>("HorizontalAlignment");
		static inline const PropertyId VerticalAligmentProperty = PropertyManager::Register<VerticalAligment>("VerticalAligment");
		static inline const PropertyId VisibilityProperty = PropertyManager::Register<Visibility>("Visibility");

		SDL_FPoint Position = { 0, 0 };
	protected:
		std::unordered_map<PropertyId, Binding> _bindings;
	private:
		bool _mouseOver = false;
	public:
		explicit Component(SDL_FPoint position = {});
		virtual ~Component() = default;		

		Event<MouseEventHandler> MouseEnter;
		Event<MouseEventHandler> MouseLeave;

		void SetWidth(float value) { SetProperty(WidthProperty, value); }
		void SetHeight(float value) { SetProperty(HeightProperty, value); }
		void SetMinWidth(float value) { SetProperty(MinWidthProperty, value); }
		void SetMinHeight(float value) { SetProperty(MinHeightProperty, value); }
		void SetMaxWidth(float value) { SetProperty(MaxWidthProperty, value); }
		void SetMaxHeight(float value) { SetProperty(MaxHeightProperty, value); }
		void SetZIndex(size_t value) { SetProperty(ZIndexProperty, value); }
		void SetMargin(Thikness value) { SetProperty(MarginProperty, value); }
		void SetHorizontalAlignment(HorizontalAlignment value) { SetProperty(HorizontalAlignmentProperty, value); }
		void SetVerticalAlignment(VerticalAligment value) { SetProperty(VerticalAligmentProperty, value); }
		void SetVisibility(Visibility value) { SetProperty(VisibilityProperty, value); }
		void SetCursor(const Sgl::Cursor* value) { SetProperty(CursorProperty, value); }
		void SetToolTip(IVisual* value) { SetProperty(ToolTipProperty, value); }
		void SetStyle(const Style& style) override;

		const float& GetWidth() const { return GetPropertyValue<float>(WidthProperty); }
		const float& GetHeight() const { return GetPropertyValue<float>(HeightProperty); }
		const float& GetMinWidth() const { return GetPropertyValue<float>(MinWidthProperty); }
		const float& GetMinHeight() const { return GetPropertyValue<float>(MinHeightProperty); }
		const float& GetMaxWidth() const { return GetPropertyValue<float>(MaxWidthProperty); }
		const float& GetMaxHeight() const { return GetPropertyValue<float>(MaxHeightProperty); }
		const size_t& GetZIndex() const { return GetPropertyValue<size_t>(ZIndexProperty); }
		const Thikness& GetMargin() const { return GetPropertyValue<Thikness>(MarginProperty); }
		const HorizontalAlignment& GetHorizontalAlignment() const { return GetPropertyValue<HorizontalAlignment>(HorizontalAlignmentProperty); }
		const VerticalAligment& GetVerticalAlignment() const { return GetPropertyValue<VerticalAligment>(VerticalAligmentProperty); }
		const Visibility& GetVisibility() const { return GetPropertyValue<Visibility>(VisibilityProperty); }
		const Cursor* const& GetCursor() const { return GetPropertyValue<const Cursor*>(CursorProperty); }
		const IVisual* const& GetToolTip() const { return GetPropertyValue<IVisual*>(ToolTipProperty); }
		
		void OnRender(RenderContext& renderContext) override;
		bool IsMouseOver() const noexcept { return _mouseOver; }
		void OnPropertyChanged(PropertyId id) override;

		template<typename T>
		void Bind(PropertyId id, T& member)
		{
			Binding& binding = _bindings[id];
			binding.Target = [&member](const Any& value)
			{
				member = value.As<T>();
			};
		}

		template<typename TTarget, typename TMember>
		void Bind(PropertyId id, void (TTarget::* setter)(const TMember&), TTarget& target,
				  BindingMode mode = BindingMode::OneWayToTarget)
		{	
			Binding& binding = _bindings[id];

			if(mode == BindingMode::OneWayToTarget || mode == BindingMode::TwoWay)
			{
				binding.Target = [set = std::bind_front(setter, &target)] (const Any& value)
				{
					set(value.As<TMember>());
				};
			}
			
			if(mode == BindingMode::OneWayToTarget)
			{
				return;
			}

			if constexpr(std::is_base_of_v<ISupportSourceBinding, TTarget>)
			{
				binding.Source = [this](PropertyId id, const Any& value)
				{
					SetProperty(id, value.As<TMember>());
				};
				target.GetNotifySource() = std::bind_front(&Binding::NotifySource, &binding);
			}
		}
	protected:
		virtual void OnMouseEnter(const MouseButtonEventArgs& e);
		virtual void OnMouseLeave(const MouseButtonEventArgs& e);
	};

	struct ZIndexComparer
	{
		bool operator()(const Component& left, const Component& right) const
		{
			return left.GetZIndex() < right.GetZIndex();
		}
	};

	template<typename T>
	using ComponentSet = std::multiset<std::reference_wrapper<T>, ZIndexComparer>;
}