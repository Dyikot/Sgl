#include "RangeBase.h"

namespace Sgl::UIElements
{
	RangeBase::RangeBase(RangeBase&& other) noexcept:
		TemplatedUIElement(std::move(other)),
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
				SetValue(_minValue, source);
			}
		}
	}

	void RangeBase::SetMaxValue(float value, ValueSource source)
	{
		if(SetProperty(MaxValueProperty, _maxValue, value, _maxValueSource, source))
		{
			if(_maxValue < _value)
			{
				SetValue(_maxValue, source);
			}
		}
	}

	void RangeBase::SetValue(float value, ValueSource source)
	{
		value = Clamp(value);

		if(SetProperty(ValueProperty, _value, value, _valueSource, source))
		{
			OnValueChanged(value);
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

	void RangeBase::OnValueChanged(float value)
	{
		ValueChanged.Invoke(*this, value);
	}

	float RangeBase::ValueAtPosition(float x, float y) const
	{
		auto bounds = GetBounds();
		return _orientation == Orientation::Horizontal
			? (_maxValue - _minValue) * (x - bounds.x) / bounds.w
			: (_maxValue - _minValue) * (y - bounds.y) / bounds.h;
	}

	float RangeBase::Clamp(float value) const
	{
		return std::clamp(value, _minValue, _maxValue);
	}
}

