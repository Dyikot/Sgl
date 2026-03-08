#pragma once

#include <vector>
#include <concepts>
#include "StyleableProperty.h"
#include "INotifyPropertyChanged.h"
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

	template<CProperty TTargetProperty, CProperty TSourceProperty, typename TConverter>
	class ConvertibleBinding;

	class BindableObject : public INotifyPropertyChanged
	{
	public:
		BindableObject() = default;
		BindableObject(const BindableObject& other);
		BindableObject(BindableObject&& other) noexcept;
		~BindableObject();	

		void SetDataContext(const Ref<INotifyPropertyChanged>& value, ValueSource source = ValueSource::Local);
		const Ref<INotifyPropertyChanged>& GetDataContext() const { return _dataContext; }

		template<CProperty TProperty, typename TField>
		bool SetProperty(TProperty& property, TField& field, TProperty::Value value,
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

		template<CProperty TProperty, typename TField>
		bool SetProperty(TProperty& property, TField& field, TProperty::Value value)
		{
			if(field == value)
			{
				return false;
			}

			field = value;
			NotifyPropertyChanged(property);

			return true;
		}

		template<CProperty TTargetProperty, CProperty TSourceProperty>
		void Bind(TTargetProperty& targetProperty,
				  TSourceProperty& sourceProperty,
				  BindingMode mode = BindingMode::OneWay)
		{
			_bindings.emplace_back(new Binding<TTargetProperty, TSourceProperty>(
				targetProperty, sourceProperty, mode));
		}

		template<CProperty TTargetProperty, CProperty TSourceProperty, typename TConverter>
		void Bind(TTargetProperty& targetProperty,
				  TSourceProperty& sourceProperty,
				  TConverter converter,
				  BindingMode mode = BindingMode::OneWay)
		{
			_bindings.emplace_back(new ConvertibleBinding<TTargetProperty, TSourceProperty, TConverter>(
				targetProperty, sourceProperty, converter, mode));
		}

		void ClearBinding(PropertyBase& targetProperty);

		static inline StyleableProperty DataContextProperty { &SetDataContext, &GetDataContext };
	protected:
		virtual void NotifyPropertyChanged(PropertyBase& property);
		virtual void OnDataContextChanged(const Ref<INotifyPropertyChanged>& dataContext) {}
		void ApplyBindings();
		void ClearBindings();
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
		virtual PropertyBase& GetTarget() const = 0;
	protected:
		bool _applied = false;
	};

	template<CProperty TTargetProperty, CProperty TSourceProperty>
	struct PropertyChangedHandler
	{
		using TTarget = TTargetProperty::Owner;
		using TSource = TSourceProperty::Owner;

		TTargetProperty& TargetProperty;
		TTarget* Target;
		TSourceProperty& SourceProperty;

		void operator()(INotifyPropertyChanged& sender, PropertyBase& e)
		{
			if(e == SourceProperty)
			{
				auto& source = static_cast<TSource&>(sender);
				TargetProperty.InvokeSetter(*Target, SourceProperty.InvokeGetter(source));
			}
		}

		bool operator==(const PropertyChangedHandler& other) const
		{
			return TargetProperty == other.TargetProperty &&
				   Target == other.Target &&
				   SourceProperty == other.SourceProperty;
		}
	};

	template<CProperty TTargetProperty, CProperty TSourceProperty>
	class Binding final : public BindingBase
	{
	public:
		using TTarget = TTargetProperty::Owner;
		using TSource = TSourceProperty::Owner;
	public:
		Binding(TTargetProperty& targetProperty, TSourceProperty& sourceProperty, BindingMode mode):
			_targetProperty(targetProperty),
			_sourceProperty(sourceProperty),
			_mode(mode)
		{}

		void Apply(BindableObject& bindableObject) override
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
					_targetProperty, target, _sourceProperty
				);
			}

			if(_mode == BindingMode::OneWayToSource || _mode == BindingMode::TwoWay)
			{
				_sourceProperty.InvokeSetter(*source, _targetProperty.InvokeGetter(*target));
				target->PropertyChanged += PropertyChangedHandler(
					_sourceProperty, source, _targetProperty
				);
			}

			_applied = true;
		}

		void Clear(BindableObject& bindableObject) override
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
					_targetProperty, target, _sourceProperty
				);
			}

			if(_mode == BindingMode::OneWayToSource || _mode == BindingMode::TwoWay)
			{
				target->PropertyChanged -= PropertyChangedHandler(
					_sourceProperty, source, _targetProperty
				);
			}

			_applied = false;
		}

		PropertyBase& GetTarget() const override
		{
			return _targetProperty;
		}
	private:
		TTargetProperty& _targetProperty;
		TSourceProperty& _sourceProperty;
		BindingMode _mode;
	};

	template<CProperty TTargetProperty, CProperty TSourceProperty, typename TConverter>
	struct ConvertiblePropertyChangedHandler
	{
		using TTarget = TTargetProperty::Owner;
		using TSource = TSourceProperty::Owner;
		using TTargetValue = TTargetProperty::Value;
		using TSourceValue = TSourceProperty::Value;

		TTargetProperty& TargetProperty;
		TTarget* Target;
		TSourceProperty& SourceProperty;
		TConverter Converter;

		void operator()(INotifyPropertyChanged& sender, PropertyBase& e)
		{
			if(e == SourceProperty)
			{
				auto& source = static_cast<TSource&>(sender);
				auto sourceValue = SourceProperty.InvokeGetter(source);
				TargetProperty.InvokeSetter(*Target, Converter(sourceValue));
			}
		}

		bool operator==(const ConvertiblePropertyChangedHandler& other) const
		{
			return TargetProperty == other.TargetProperty &&
				   Target == other.Target &&
				   SourceProperty == other.SourceProperty;
		}
	};

	template<CProperty TTargetProperty, CProperty TSourceProperty, typename TConverter>
	class ConvertibleBinding final : public BindingBase
	{
	public:
		using TTarget = TTargetProperty::Owner;
		using TSource = TSourceProperty::Owner;
		using TTargetValue = TTargetProperty::Value;
		using TSourceValue = TSourceProperty::Value;
	public:
		ConvertibleBinding(TTargetProperty& targetProperty, TSourceProperty& sourceProperty, 
						   TConverter converter, BindingMode mode):
			_targetProperty(targetProperty),
			_sourceProperty(sourceProperty),
			_converter(converter),
			_mode(mode)
		{}

		void Apply(BindableObject& bindableObject) override
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
				auto sourceValue = _sourceProperty.InvokeGetter(*source);
				_targetProperty.InvokeSetter(*target, _converter(sourceValue));

				source->PropertyChanged += ConvertiblePropertyChangedHandler<
					TTargetProperty, TSourceProperty, TConverter>(
						_targetProperty, target, _sourceProperty, _converter
				);
			}

			if(_mode == BindingMode::OneWayToSource || _mode == BindingMode::TwoWay)
			{
				auto targetValue = _targetProperty.InvokeGetter(*target);
				_sourceProperty.InvokeSetter(*source, _converter(targetValue));

				target->PropertyChanged += ConvertiblePropertyChangedHandler<
					TSourceProperty, TTargetProperty, TConverter>(
						_sourceProperty, source, _targetProperty, _converter
				);
			}

			_applied = true;
		}

		void Clear(BindableObject& bindableObject) override
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
				source->PropertyChanged -= ConvertiblePropertyChangedHandler<
					TTargetProperty, TSourceProperty, TConverter>(
						_targetProperty, target, _sourceProperty, _converter
				);
			}

			if(_mode == BindingMode::OneWayToSource || _mode == BindingMode::TwoWay)
			{
				target->PropertyChanged -= ConvertiblePropertyChangedHandler<
					TSourceProperty, TTargetProperty, TConverter>(
						_sourceProperty, source, _targetProperty, _converter
				);
			}

			_applied = false;
		}

		PropertyBase& GetTarget() const override
		{
			return _targetProperty;
		}
	private:
		TTargetProperty& _targetProperty;
		TSourceProperty& _sourceProperty;
		TConverter _converter;
		BindingMode _mode;
	};
}