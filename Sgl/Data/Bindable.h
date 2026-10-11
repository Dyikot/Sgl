#pragma once

#include <vector>

#include "Binding.h"
#include "StyleableProperty.h"
#include "../Base/Any.h"
#include "../Base/Collections/FlatMap.h"

namespace Sgl
{
    class Bindable : public ObservableObject
    {
    public:
        struct PropertyValue
        {
            Any state;
        };

    public:
        Bindable() = default;
        Bindable(const Bindable&) = delete;
        Bindable(Bindable&&) = delete;
        
        void SetDataContext(const Ref<ObservableObject>& value, ValueSource source = ValueSource::Local);
        const Ref<ObservableObject>& GetDataContext() const { return _dataContext; }	

        template<CProperty TTargetProperty, CProperty TSourceProperty>
        void Bind(TTargetProperty& targetProperty,
                  TSourceProperty& sourceProperty,
                  BindingMode mode = BindingMode::OneWay)
        {
            auto binding = new Binding(targetProperty, sourceProperty, mode);
            _bindings.emplace_back(binding);
        }

        template<CProperty TTargetProperty, CProperty TSourceProperty, typename TConverter>
        void Bind(TTargetProperty& targetProperty,
                  TSourceProperty& sourceProperty,
                  TConverter converter,
                  BindingMode mode = BindingMode::OneWay)
        {
            auto binding = new ConvertibleBinding(targetProperty, sourceProperty, converter, mode);
            _bindings.emplace_back(binding);
        }

        void ClearBinding(PropertyBase& targetProperty);

        static inline StyleableProperty DataContextProperty { &SetDataContext, &GetDataContext };
    protected:
        ~Bindable();

        using ObservableObject::SetProperty;

        template<CProperty TProperty, typename TField>
        bool SetProperty(TProperty& property, TField& field, TProperty::Value value,
                         ValueSource& currentSource, ValueSource newSource)
        {
            if(currentSource > newSource)
            {
                return false;
            }

            if(newSource < ValueSource::VisualState)
            {
                if(field == value)
                {
                    currentSource = newSource;
                    return false;
                }

                field = value;
                currentSource = newSource;
            }
            else
            {
                PropertyValue  newValue = { .state = value };
                PropertyValue* currentValue = _valueStorage.Find(&property);

                if(currentValue && currentValue->state == newValue.state)
                {
                    return false;
                }

                _valueStorage.Add(&property, std::move(newValue));
            }

            OnPropertyChanged(property);
            return true;
        }

        template<CProperty TProperty, typename TField>
        TProperty::Value GetProperty(TProperty& property, const TField& field) const
        {
            const PropertyValue* it = _valueStorage.Find(&property);
            return it ? it->state.As<TField>() : field;
        }

        void ClearValue(PropertyBase& property);
        void ClearAllValues();
        void OnPropertyChanged(PropertyBase& property) override;
        void ApplyBindings();
        void ClearBindings();
        virtual void OnDataContextChanged(const Ref<ObservableObject>& dataContext) {}
    private:
        std::vector<std::unique_ptr<BindingBase>> _bindings;
        std::vector<PropertyBase*> _clearProperties;
        FlatMap<PropertyBase*, PropertyValue> _valueStorage;
        Ref<ObservableObject> _dataContext;

        ValueSource _dataContextSource {};
    };
}