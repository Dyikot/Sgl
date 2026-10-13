#pragma once

#include <vector>

#include "Binding.h"
#include "StyleableProperty.h"
#include "../Base/Tools/Concepts.h"
#include "../Input/Cursor.h"
#include "../Base/Media/Brush.h"
#include "../Base/Collections/FlatMap.h"

namespace Sgl
{
    //! @brief Determines the priority when setting the value
    enum class ValueSource: uint8_t
    {
        Default,
        Inheritance,
        Style,
        VisualState,
        Local
    };

    class Bindable : public ObservableObject
    {
    public:
        using PropertyValue = std::variant<uint32_t, Color, Brush, Cursor>;
    public:
        Bindable() = default;
        Bindable(const Bindable&) = delete;
        Bindable(Bindable&&) = delete;
        
        void SetValueSource(ValueSource valueSource);
        ValueSource GetValueSource() const { return _valueSource; }

        void SetDataContext(const Ref<ObservableObject>& value);
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
        bool SetProperty(TProperty& property, TField& field, TProperty::Value value, ValueSource& currentSource)
        {
            if(currentSource > _valueSource)
            {
                return false;
            }

            if(_valueSource != ValueSource::VisualState)
            {
                if(field == value)
                {
                    currentSource = _valueSource;
                    return false;
                }

                field = value;
                currentSource = _valueSource;
            }
            else
            {
                if constexpr(CVariantAlternative<TField, PropertyValue>)
                {
                    PropertyValue newValue = value;
                    PropertyValue* currentValue = _valueStorage.Find(&property);

                    if(currentValue && *currentValue == newValue)
                    {
                        return false;
                    }

                    _valueStorage.Add(&property, std::move(newValue));
                }
                else
                {
                    throw Exception("Property does not support VisualState value source");
                }
            }

            OnPropertyChanged(property);
            return true;
        }

        template<CProperty TProperty, typename TField>
        TProperty::Value GetProperty(TProperty& property, const TField& field) const
        {
            const PropertyValue* it = _valueStorage.Find(&property);
            return it ? std::get<TField>(*it) : field;
        }

        void ClearValue(PropertyBase& property);
        void ClearAllValues();
        void OnPropertyChanged(PropertyBase& property) override;
        void ApplyBindings();
        void ClearBindings();
        virtual void OnDataContextChanged(const Ref<ObservableObject>& dataContext) {}
    private:
        std::vector<std::unique_ptr<BindingBase>> _bindings;
        FlatMap<PropertyBase*, PropertyValue> _valueStorage;
        Ref<ObservableObject> _dataContext;

        ValueSource _valueSource = ValueSource::Local;
        ValueSource _dataContextSource {};
    };

    class ValueSourceScope
    {
    public:
        ValueSourceScope(Bindable& target, ValueSource valueSource);
        ValueSourceScope(const Ref<Bindable>& target, ValueSource valueSource);
        ~ValueSourceScope();
    private:
        Bindable& _target;
        ValueSource _currentSource;
    };
}