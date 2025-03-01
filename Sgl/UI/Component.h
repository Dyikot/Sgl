#pragma once

#include <unordered_map>
#include <set>
#include "UIElement.h"
#include "../Style/Properties.h"
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
		static inline const PropertyId ToolTipProperty = PropertyManager::Register<IVisual*>("ToolTip");
		static inline const PropertyId HorizontalAlignmentProperty = PropertyManager::Register<HorizontalAlignment>("HorizontalAlignment");
		static inline const PropertyId VerticalAligmentProperty = PropertyManager::Register<VerticalAligment>("VerticalAligment");
		static inline const PropertyId VisibilityProperty = PropertyManager::Register<Visibility>("Visibility");

		UIElement& Parent;
		SDL_FPoint Position = { 0, 0 };
	private:
		bool _mouseOver = false;
		void* _dataContext = nullptr;
		std::unordered_map<PropertyId, Binding> _bindings;
	public:
		explicit Component(UIElement& parent);
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
		void SetToolTip(IVisual* value) { SetProperty(ToolTipProperty, value); }

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
		const IVisual* const& GetToolTip() const { return GetPropertyValue<IVisual*>(ToolTipProperty); }
		
		void OnRender(RenderContext& renderContext) override;
		void AddStyle(const Style& style) override;
		void SwitchCursorOn(const Cursor& cursor) override;
		void SwitchCursorOnDefault() override;
		bool IsMouseOver() const noexcept { return _mouseOver; }
		void OnPropertyChanged(PropertyId id) override;	

		template<typename TObject, typename TMember>
		void Bind(PropertyId id, void (TObject::* setter)(const TMember&))
		{
			auto& binding = _bindings[id];

			binding.Target = [this, setter](const Any& value)
			{
				(*static_cast<TObject*>(_dataContext).*setter)(value.As<TMember>());
			};
		}

		template<typename TObject, typename TMember>
			requires std::derived_from<TObject, ISupportComponentBinding>
		void Bind(PropertyId id, const TMember& (TObject::* getter)() const)
		{
			auto& binding = _bindings[id];

			binding.Source = [this, getter](PropertyId id)
			{
				SetProperty<TMember>(id, (*static_cast<TObject*>(_dataContext).*getter)());
			};
		}

		template<typename TObject, typename TMember>
		void Bind(PropertyId id,
				  void (TObject::* setter)(const TMember&),
				  const TMember& (TObject::* getter)() const)
		{
			Bind(id, setter);
			Bind(id, getter);
		}

		template<typename TObject>
		void SetDataContext(TObject& object)
		{
			if constexpr(std::is_base_of_v<ISupportComponentBinding, TObject>)
			{
				if(_dataContext)
				{
					static_cast<TObject*>(_dataContext)->GetSourceNotifier() = nullptr;
				}
			}

			_dataContext = &object;

			if constexpr(std::is_base_of_v<ISupportComponentBinding, TObject>)
			{
				object.GetSourceNotifier() = std::bind_front(&Component::NotifySource, this);
			}
		}
	protected:
		virtual void OnMouseEnter(const MouseButtonEventArgs& e);
		virtual void OnMouseLeave(const MouseButtonEventArgs& e);
	private:
		void NotifySource(PropertyId id);
		void NotifyTarget(PropertyId id, const Any& value);
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