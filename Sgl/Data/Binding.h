#pragma once

#include "Property.h"
#include "ObservableObject.h"
#include "../Base/Exceptions.h"

namespace Sgl
{
    class Bindable;

    enum class BindingMode
    {
        OneWay, OneWayToSource, TwoWay
    };

    class BindingBase
    {
    public:
        virtual ~BindingBase() = default;

        bool IsApplied() const noexcept { return _applied; }

        virtual void Apply(Bindable& bindableObject) = 0;
        virtual void Clear(Bindable& bindableObject) = 0;
        virtual PropertyBase& GetTarget() const = 0;
    protected:
        static const Ref<ObservableObject>& GetDataContext(const Bindable& bindableObject);
    protected:
        bool _applied = false;
    };

    template<typename TTargetProperty, typename TSourceProperty>
    struct BindingHandler
    {
        using TTarget = TTargetProperty::Owner;
        using TSource = TSourceProperty::Owner;

        TTargetProperty& TargetProperty;
        TTarget* Target;
        TSourceProperty& SourceProperty;

        void operator()(ObservableObject& sender, PropertyBase& e)
        {
            if(e == SourceProperty)
            {
                auto& source = static_cast<TSource&>(sender);
                TargetProperty.InvokeSetter(*Target, SourceProperty.InvokeGetter(source));
            }
        }

        bool operator==(const BindingHandler& other) const
        {
            return TargetProperty == other.TargetProperty &&
                   Target == other.Target &&
                   SourceProperty == other.SourceProperty;
        }
    };

    template<CProperty TTargetProperty, CProperty TSourceProperty>
    class Binding final: public BindingBase
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

        void Apply(Bindable& bindableObject) override
        {
            auto& dataContext = GetDataContext(bindableObject);

            if(!dataContext)
            {
                throw Exception("Unable to set a binding. Data context is null.");
            }

            auto target = static_cast<TTarget*>(&bindableObject);
            auto source = dataContext.GetAs<TSource>();

            if(_mode == BindingMode::OneWay || _mode == BindingMode::TwoWay)
            {
                _targetProperty.InvokeSetter(*target, _sourceProperty.InvokeGetter(*source));
                source->PropertyChanged += BindingHandler(_targetProperty, target, _sourceProperty);
            }

            if(_mode == BindingMode::OneWayToSource || _mode == BindingMode::TwoWay)
            {
                _sourceProperty.InvokeSetter(*source, _targetProperty.InvokeGetter(*target));
                target->PropertyChanged += BindingHandler(_sourceProperty, source, _targetProperty);
            }

            _applied = true;
        }

        void Clear(Bindable& bindableObject) override
        {
            auto& dataContext = GetDataContext(bindableObject);

            if(!dataContext)
            {
                throw Exception("Unable to set a binding. Data context is null.");
            }

            auto target = static_cast<TTarget*>(&bindableObject);
            auto source = dataContext.GetAs<TSource>();

            if(_mode == BindingMode::OneWay || _mode == BindingMode::TwoWay)
            {
                source->PropertyChanged -= BindingHandler(_targetProperty, target, _sourceProperty);
            }

            if(_mode == BindingMode::OneWayToSource || _mode == BindingMode::TwoWay)
            {
                target->PropertyChanged -= BindingHandler(_sourceProperty, source, _targetProperty);
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

    template<typename TTargetProperty, typename TSourceProperty, typename TConverter>
    struct ConvertibleBindingHandler
    {
        using TTarget = TTargetProperty::Owner;
        using TSource = TSourceProperty::Owner;
        using TTargetValue = TTargetProperty::Value;
        using TSourceValue = TSourceProperty::Value;

        TTargetProperty& TargetProperty;
        TTarget* Target;
        TSourceProperty& SourceProperty;
        TConverter Converter;

        void operator()(ObservableObject& sender, PropertyBase& e)
        {
            if(e == SourceProperty)
            {
                auto& source = static_cast<TSource&>(sender);
                auto sourceValue = SourceProperty.InvokeGetter(source);
                TargetProperty.InvokeSetter(*Target, Converter(sourceValue));
            }
        }

        bool operator==(const ConvertibleBindingHandler& other) const
        {
            return TargetProperty == other.TargetProperty &&
                   Target == other.Target &&
                   SourceProperty == other.SourceProperty;
        }
    };

    template<CProperty TTargetProperty, CProperty TSourceProperty, typename TConverter>
    class ConvertibleBinding final: public BindingBase
    {
    public:
        using TTarget = TTargetProperty::Owner;
        using TSource = TSourceProperty::Owner;
        using TTargetValue = TTargetProperty::Value;
        using TSourceValue = TSourceProperty::Value;

    public:
        ConvertibleBinding(TTargetProperty& targetProperty, 
                           TSourceProperty& sourceProperty,
                           TConverter converter, 
                           BindingMode mode):
            _targetProperty(targetProperty),
            _sourceProperty(sourceProperty),
            _converter(converter),
            _mode(mode)
        {}

        void Apply(Bindable& bindableObject) override
        {
            auto& dataContext = GetDataContext(bindableObject);

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

                source->PropertyChanged += ConvertibleBindingHandler<
                    TTargetProperty, TSourceProperty, TConverter>(
                        _targetProperty, target, _sourceProperty, _converter
                    );
            }

            if(_mode == BindingMode::OneWayToSource || _mode == BindingMode::TwoWay)
            {
                auto targetValue = _targetProperty.InvokeGetter(*target);
                _sourceProperty.InvokeSetter(*source, _converter(targetValue));

                target->PropertyChanged += ConvertibleBindingHandler<
                    TSourceProperty, TTargetProperty, TConverter>(
                        _sourceProperty, source, _targetProperty, _converter
                    );
            }

            _applied = true;
        }

        void Clear(Bindable& bindableObject) override
        {
            auto& dataContext = GetDataContext(bindableObject);

            if(!dataContext)
            {
                throw Exception("Unable to set a binding. Data context is null.");
            }

            auto target = static_cast<TTarget*>(&bindableObject);
            auto source = dataContext.GetAs<TSource>();

            if(_mode == BindingMode::OneWay || _mode == BindingMode::TwoWay)
            {
                source->PropertyChanged -= ConvertibleBindingHandler<
                    TTargetProperty, TSourceProperty, TConverter>(
                        _targetProperty, target, _sourceProperty, _converter
                    );
            }

            if(_mode == BindingMode::OneWayToSource || _mode == BindingMode::TwoWay)
            {
                target->PropertyChanged -= ConvertibleBindingHandler<
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