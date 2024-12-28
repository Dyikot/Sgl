#pragma once

#include <unordered_map>
#include "UIElement.h"
#include "../Appearance/UIAppearance.h"
#include "../Appearance/Cursor.h"
#include "../Events/Delegates.h"

namespace Sgl
{
	class Object;

	using PropertyChangedEventHandler = EventHandler<Object, EventArgs>;

	class Object: public UIElement
	{
	public:
		static inline const PropertyId WidthProperty = PropertyManager::Register<float>("Width");
		static inline const PropertyId HeightProperty = PropertyManager::Register<float>("Height");
		static inline const PropertyId MinWidthProperty = PropertyManager::Register<float>("MinWidth");
		static inline const PropertyId MinHeightProperty = PropertyManager::Register<float>("MinHeight");
		static inline const PropertyId MaxWidthProperty = PropertyManager::Register<float>("MaxWidth");
		static inline const PropertyId MaxHeightProperty = PropertyManager::Register<float>("MaxHeight");
		static inline const PropertyId ZIndexProperty = PropertyManager::Register<size_t>("ZIndex", 1);
		static inline const PropertyId MarginProperty = PropertyManager::Register<Thikness>("Margin");
		static inline const PropertyId CursorProperty = PropertyManager::Register<Cursor*>("Cursor", nullptr);
		static inline const PropertyId ToolTipProperty = PropertyManager::Register<IVisual*>("ToolTip", nullptr);
		static inline const PropertyId HorizontalAlignmentProperty = PropertyManager::Register<HorizontalAlignment>("HorizontalAlignment", HorizontalAlignment::Stretch);
		static inline const PropertyId VerticalAligmentProperty = PropertyManager::Register<VerticalAligment>("VerticalAligment", VerticalAligment::Stretch);
		static inline const PropertyId VisibilityProperty = PropertyManager::Register<Visibility>("Visibility", Visibility::Visible);
		static inline const EventId MouseEnterEvent = EventManager::Register<MouseEventHandler>("MouseEnter");
		static inline const EventId MouseLeaveEvent = EventManager::Register<MouseEventHandler>("MouseLeave");

		SDL_FPoint Position = { 0, 0 };
		Cursor* Cursor = nullptr;
		IVisual* ToolTip = nullptr;
	protected:
		PropertySetterMap _properties = PropertyManager::GetDefaultProperties();
		std::unordered_map<PropertyId, Event<PropertyChangedEventHandler>> _propertyChangedEventMap;
	private:
		bool _isMouseOver = false;
	public:
		Object();
		explicit Object(const Style& style, SDL_FPoint position = {0, 0}) noexcept;
		virtual ~Object() = default;

		void SetWidth(float value);
		void SetHeight(float value);
		void SetMinWidth(float value);
		void SetMinHeight(float value);
		void SetMaxWidth(float value);
		void SetMaxHeight(float value);
		void SetZIndex(size_t value);
		void SetMargin(const Thikness& value);
		void SetHorizontalAlignment(HorizontalAlignment value);
		void SetVerticalAlignment(VerticalAligment value);
		void SetVisibility(Visibility value);

		float GetWidth() const { return _properties.At(WidthProperty).As<float>(); }
		float GetHeight() const { return _properties.At(HeightProperty).As<float>(); }
		float GetMinWidth() const { return _properties.At(MinWidthProperty).As<float>(); }
		float GetMinHeight() const { return _properties.At(MinHeightProperty).As<float>(); }
		float GetMaxWidth() const { return _properties.At(MaxWidthProperty).As<float>(); }
		float GetMaxHeight() const { return _properties.At(MaxHeightProperty).As<float>(); }
		size_t GetZIndex() const { return _properties.At(ZIndexProperty).As<size_t>(); }
		const Thikness& GetMargin() const { return _properties.At(MarginProperty).As<Thikness>(); }
		HorizontalAlignment GetHorizontalAlignment() const { return _properties.At(HorizontalAlignmentProperty).As<HorizontalAlignment>(); }
		VerticalAligment GetVerticalAlignment() const { return _properties.At(VerticalAligmentProperty).As<VerticalAligment>(); }
		Visibility GetVisibility() const { return _properties.At(VisibilityProperty).As<Visibility>(); }

		Event<MouseEventHandler> MouseEnter;
		Event<MouseEventHandler> MouseLeave;

		void OnRender(RenderContext& renderContext) override;
		bool IsMouseOver() const noexcept { return _isMouseOver; }

		void AddPropertyChangedHandler(PropertyId id, auto&& handler)
		{
			_propertyChangedEventMap[id] += std::forward<decltype(handler)>(handler);
		}

		void RemovePropertyChangedHandler(PropertyId id, auto&& handler)
		{
			_propertyChangedEventMap[id] -= std::forward<decltype(handler)>(handler);
		}

		template<typename T>
		void Bind(PropertyId id, T& item)
		{
			_propertyChangedEventMap[id] +=
				[&item, property = &_properties[id].As<T>()] (Object* sender, const EventArgs& e)
				{
					item = *property;
				};
		}
	protected:
		virtual void OnMouseEnter(const MouseButtonEventArgs& e);
		virtual void OnMouseLeave(const MouseButtonEventArgs& e);
		void OnPropertyChanged(PropertyId id);
	private:
		void InitPropertyChangedMap();
	};

	struct ZIndexComparer
	{
		bool operator()(const Object* left, const Object* right) const
		{
			return left->GetZIndex() < right->GetZIndex();
		}
	};
}