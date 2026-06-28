#include "RangeBase.h"

namespace Sgl::UIElements
{
	RangeBase::RangeBase(RangeBase&& other) noexcept:
		UIElement(std::move(other)),
		_minValue(other._minValue),
		_maxValue(other._maxValue),
		_value(other._value),
		_orientation(other._orientation)
	{}

	void RangeBase::SetMinValue(float value, ValueSource source)
	{
		if(SetProperty(MinValueProperty, _minValue, value, _minValueSource, source))
		{
			if(_minValue > _value)
			{
				SetValue(_minValue);
			}

			InvalidateRender();
		}
	}

	void RangeBase::SetMaxValue(float value, ValueSource source)
	{
		if(SetProperty(MaxValueProperty, _maxValue, value, _maxValueSource, source))
		{
			if(_maxValue < _value)
			{
				SetValue(_maxValue);
			}

			InvalidateRender();
		}
	}

	void RangeBase::SetValue(float value, ValueSource source)
	{
		value = std::clamp(value, _minValue, _maxValue);

		if(SetProperty(ValueProperty, _value, value, _valueSource, source))
		{
			InvalidateRender();
			ValueChanged.Invoke(*this, value);
		}
	}

	void RangeBase::SetOrientation(Orientation value, ValueSource source)
	{
		if(SetProperty(OrientationProperty, _orientation, value, _orientationSource, source))
		{
			InvalidateRender();
		}
	}

	float RangeBase::GetRelativeValue() const
	{
		if(_maxValue - _minValue < 1e-6f)
		{
			return 0;
		}

		return (_value - _minValue) / (_maxValue - _minValue);
	}
}

