#pragma once

#include "../UIElement.h"

namespace Sgl::UIElements
{
	class RangeBase : public UIElement
	{
	public:
		using ValueChangedEventHandler = EventHandler<RangeBase, float>;
	public:
		RangeBase() = default;
		RangeBase(RangeBase&& other) noexcept;

		Event<ValueChangedEventHandler> ValueChanged;

		void SetMinValue(float value, ValueSource source = ValueSource::Local);
		float GetMinValue() const { return _minValue; }

		void SetMaxValue(float value, ValueSource source = ValueSource::Local);
		float GetMaxValue() const { return _maxValue; }

		void SetValue(float value, ValueSource source = ValueSource::Local);
		float GetValue() const { return _value; }

		void SetOrientation(Orientation value, ValueSource source = ValueSource::Local);
		Orientation GetOrientation() const { return _orientation; }

		float GetRelativeValue() const;

		static inline StyleableProperty MinValueProperty { &SetMinValue, &GetMinValue };
		static inline StyleableProperty MaxValueProperty { &SetMaxValue, &GetMaxValue };
		static inline StyleableProperty ValueProperty { &SetValue, &GetValue };
		static inline StyleableProperty OrientationProperty { &SetOrientation, &GetOrientation };
	private:
		float _minValue = 0;
		float _maxValue = 100;
		float _value = _minValue;
		Orientation _orientation = Orientation::Horizontal;

		ValueSource _minValueSource {};
		ValueSource _maxValueSource {};
		ValueSource _valueSource {};
		ValueSource _orientationSource {};
	};
}