#pragma once

#include <vector>
#include <concepts>
#include "Converters.h"
#include "StyleableProperty.h"
#include "INotifyPropertyChanged.h"
#include "AttachPropertiesRegister.h"
#include "../Base/Ref.h"
#include "../Base/Exceptions.h"

namespace Sgl
{
	enum class BindingMode
	{
		OneWay, OneWayToSource, TwoWay
	};	

	class BindingBase;

	template<CProperty TTargetProperty, CProperty TSourceProperty>
	class Binding;

	class BindableObject : public INotifyPropertyChanged
	{
	private:
		using DestroyingEventHandler = EventHandler<BindableObject>;	
	public:
		BindableObject() = default;
		BindableObject(const BindableObject& other);
		BindableObject(BindableObject&& other) noexcept;
		~BindableObject();	

		Event<DestroyingEventHandler> Destroying;

		void SetDataContext(const Ref<INotifyPropertyChanged>& value, ValueSource source = ValueSource::Local);
		const Ref<INotifyPropertyChanged>& GetDataContext() const { return _dataContext; }

		template<CProperty TTargetProperty, CProperty TSourceProperty>
		void Bind(TTargetProperty& targetProperty,
				  TSourceProperty& sourceProperty,
				  BindingMode mode = BindingMode::OneWay)
		{
			_bindings.emplace_back(new Binding<TTargetProperty, TSourceProperty>(
				targetProperty, sourceProperty, mode));
		}

		template<typename... TArgs>
		void UseAttachedProperties()
		{
			((Destroying += &ClearAttachedProperty<TArgs>), ...);
		}

		template<typename T, typename TValue, typename TSetValue>
			requires std::constructible_from<TSetValue, TValue>
		void SetAttachedValue(TValue T::* field, const TSetValue& value)
		{
			auto& reg = AttachPropertiesRegister<T>::Get();
			auto& property = reg.GetAttachedProperty(this);
			property.*field = value;
		}

		template<typename T, typename TValue>
		TValue GetAttachedValue(TValue T::* field) const
		{
			auto& reg = AttachPropertiesRegister<T>::Get();
			auto& property = reg.GetAttachedProperty(this);
			return property.*field;
		}

		static inline StyleableProperty DataContextProperty { &SetDataContext, &GetDataContext };
	protected:
		virtual void NotifyPropertyChanged(PropertyBase& property);
		virtual void OnDataContextChanged(const Ref<INotifyPropertyChanged>& dataContext) {}
		void ApplyBindings();
		void ClearBindings();

		template<typename TOwner, typename TValue, typename TField>
		bool SetProperty(StyleableProperty<TOwner, TValue>& property, TField& field,
						 StyleableProperty<TOwner, TValue>::Value value,
						 ValueSource& currentSource, ValueSource newSource)
		{
			if(currentSource > newSource || field == value)
			{
				return false;
			}

			field = value;
			currentSource = newSource;
			NotifyPropertyChanged(property);

			return true;
		}
	private:
		template<typename T>
		static void ClearAttachedProperty(BindableObject& sender, EventArgs e)
		{
			using TProperties = T::AttachedProperties;
			auto& reg = AttachPropertiesRegister<TProperties>::Get();
			reg.ClearAttachedProperty(&sender);
		}
	private:
		std::vector<std::unique_ptr<BindingBase>> _bindings;
		Ref<INotifyPropertyChanged> _dataContext;

		ValueSource _dataContextSource {};
	};	

	class BindingBase
	{
	public:
		virtual ~BindingBase() = default;

		bool IsApplied() const noexcept { return _applied; }

		virtual void Apply(BindableObject& bindableObject) = 0;
		virtual void Clear(BindableObject& bindableObject) = 0;
	protected:
		bool _applied = false;
	};

	template<CProperty TTargetProperty, CProperty TSourceProperty>
	struct PropertyChangedHandler
	{
		using TTarget = TTargetProperty::Owner;
		using TSource = TSourceProperty::Owner;
		using TTargetValue = TTargetProperty::Value;
		using TSourceValue = TSourceProperty::Value;

		TTargetProperty& TargetProperty;
		TTarget* Target;
		TSourceProperty& SourceProperty;
		TSource* Source;

		void operator()(INotifyPropertyChanged& sender, PropertyBase& e)
		{
			if(e == TargetProperty)
			{
				TargetProperty.InvokeSetter(*Target, SourceProperty.InvokeGetter(*Source));
			}
		}

		bool operator==(const PropertyChangedHandler& other) const
		{
			return TargetProperty == other.TargetProperty &&
				   Target == other.Target &&
				   SourceProperty == other.SourceProperty &&
				   Source == other.Source;
		}
	};

	template<CProperty TTargetProperty, CProperty TSourceProperty>
	class Binding: public BindingBase
	{
	public:
		using TTarget = TTargetProperty::Owner;
		using TSource = TSourceProperty::Owner;
		using TTargetValue = TTargetProperty::Value;
		using TSourceValue = TSourceProperty::Value;
	public:
		Binding(TTargetProperty& targetProperty, TSourceProperty& sourceProperty, BindingMode mode):
			_targetProperty(targetProperty),
			_sourceProperty(sourceProperty),
			_mode(mode)
		{}

		void Apply(BindableObject& bindableObject) final
		{
			auto& dataContext = bindableObject.GetDataContext();

			if(!dataContext)
			{
				throw Exception("Unable to set a binding. Data context is null.");
			}

			auto target = static_cast<TTarget*>(&bindableObject);
			auto source = dataContext.GetAs<TSource>();

			if(_mode == BindingMode::OneWay || _mode == BindingMode::TwoWay)
			{
				_targetProperty.InvokeSetter(*target, _sourceProperty.InvokeGetter(*source));
				source->PropertyChanged += PropertyChangedHandler(
					_targetProperty, target, _sourceProperty, source
				);
			}

			if(_mode == BindingMode::OneWayToSource || _mode == BindingMode::TwoWay)
			{
				_sourceProperty.InvokeSetter(*source, _targetProperty.InvokeGetter(*target));
				target->PropertyChanged += PropertyChangedHandler(
					_sourceProperty, source, _targetProperty, target
				);
			}

			_applied = true;
		}

		void Clear(BindableObject& bindableObject) final
		{
			auto& dataContext = bindableObject.GetDataContext();

			if(!dataContext)
			{
				throw Exception("Unable to set a binding. Data context is null.");
			}

			auto target = static_cast<TTarget*>(&bindableObject);
			auto source = dataContext.GetAs<TSource>();

			if(_mode == BindingMode::OneWay || _mode == BindingMode::TwoWay)
			{
				source->PropertyChanged -= PropertyChangedHandler(
					_targetProperty, target, _sourceProperty, source
				);
			}

			if(_mode == BindingMode::OneWayToSource || _mode == BindingMode::TwoWay)
			{
				target->PropertyChanged -= PropertyChangedHandler(
					_sourceProperty, source, _targetProperty, target
				);
			}

			_applied = false;
		}
	private:
		TTargetProperty& _targetProperty;
		TSourceProperty& _sourceProperty;
		BindingMode _mode;
	};
}