#pragma once

#include <unordered_map>
#include "UIElement.h"
#include "../Appearance/UIAppearance.h"
#include "../Appearance/Cursor.h"
#include "../Events/Delegates.h"
#include "../Data/BindingMap.h"

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
		static inline const EventId MouseEnterEvent = EventManager::Register<MouseEventHandler>("MouseEnter");
		static inline const EventId MouseLeaveEvent = EventManager::Register<MouseEventHandler>("MouseLeave");

		SDL_FPoint Position = { 0, 0 };
		const Cursor* Cursor = nullptr;
		IVisual* ToolTip = nullptr;
	protected:
		BindingMap _bindings;
	private:
		bool _isEventsInitialized = false;
		bool _isMouseOver = false;
	public:
		Component();
		explicit Component(SDL_FPoint position) noexcept;
		virtual ~Component() = default;

		void SetWidth(float value) { SetProperty(WidthProperty, value); }
		void SetHeight(float value) { SetProperty(HeightProperty, value); }
		void SetMinWidth(float value) { SetProperty(MinWidthProperty, value); }
		void SetMinHeight(float value) { SetProperty(MinHeightProperty, value); }
		void SetMaxWidth(float value) { SetProperty(MaxWidthProperty, value); }
		void SetMaxHeight(float value) { SetProperty(MaxHeightProperty, value); }
		void SetZIndex(size_t value) { SetProperty(ZIndexProperty, value); }
		void SetMargin(const Thikness& value) { SetProperty(MarginProperty, value); }
		void SetHorizontalAlignment(HorizontalAlignment value) { SetProperty(HorizontalAlignmentProperty, value); }
		void SetVerticalAlignment(VerticalAligment value) { SetProperty(VerticalAligmentProperty, value); }
		void SetVisibility(Visibility value) { SetProperty(VisibilityProperty, value); }
		void SetStyle(const Style& style) override;

		float GetWidth() const { return GetPropertyValue<float>(WidthProperty); }
		float GetHeight() const { return GetPropertyValue<float>(HeightProperty); }
		float GetMinWidth() const { return GetPropertyValue<float>(MinWidthProperty); }
		float GetMinHeight() const { return GetPropertyValue<float>(MinHeightProperty); }
		float GetMaxWidth() const { return GetPropertyValue<float>(MaxWidthProperty); }
		float GetMaxHeight() const { return GetPropertyValue<float>(MaxHeightProperty); }
		size_t GetZIndex() const { return GetPropertyValue<size_t>(ZIndexProperty); }
		const Thikness& GetMargin() const { return GetPropertyValue<Thikness>(MarginProperty); }
		HorizontalAlignment GetHorizontalAlignment() const { return GetPropertyValue<HorizontalAlignment>(HorizontalAlignmentProperty); }
		VerticalAligment GetVerticalAlignment() const { return GetPropertyValue<VerticalAligment>(VerticalAligmentProperty); }
		Visibility GetVisibility() const { return GetPropertyValue<Visibility>(VisibilityProperty); }

		Event<MouseEventHandler>& MouseEnter;
		Event<MouseEventHandler>& MouseLeave;
		
		void OnRender(RenderContext& renderContext) override;
		bool IsMouseOver() const noexcept { return _isMouseOver; }
		void OnPropertyChanged(PropertyId id) override;

		template<typename TData, typename TMember>
			requires std::derived_from<TData, ISupportDataBinding>
		void Bind(PropertyId id, ISupportDataBinding& source, TMember TData::* member)
		{
			_bindings.Add(id, GetPropertyValue<TMember>(id), source, member);
		}

		template<typename TData, typename TMember>
			requires std::derived_from<TData, ISupportDataBinding>
		void Bind(PropertyId id, 
				  ISupportDataBinding& source,
				  TMember TData::* member, 
				  std::string_view memberName,
				  BindingMode mode = BindingMode::OneWayToSource)
		{
			_bindings.Add(id, GetPropertyValue<TMember>(id), source, member, memberName, mode);
		}

		void ClearBinding(PropertyId id);
		void ClearBindings();
	protected:
		virtual void OnMouseEnter(const MouseButtonEventArgs& e);
		virtual void OnMouseLeave(const MouseButtonEventArgs& e);
	private:
		bool InitializeEvents();
	};

	struct ZIndexComparer
	{
		bool operator()(const Component& left, const Component& right) const
		{
			return left.GetZIndex() < right.GetZIndex();
		}
	};
}