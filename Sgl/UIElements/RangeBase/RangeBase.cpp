#include "RangeBase.h"

namespace Sgl::UIElements
{
	void RangeBase::SetValue(float value, ValueSource source)
	{
		SetProperty(ValueProperty, _value, Clamp(value), _valueSource, source);
	}

	void RangeBase::SetMinValue(float value, ValueSource source)
	{
		SetProperty(MinValueProperty, _minValue, value, _minValueSource, source);
	}

	void RangeBase::SetMaxValue(float value, ValueSource source)
	{
		SetProperty(MaxValueProperty, _maxValue, value, _maxValueSource, source);
	}

	void RangeBase::SetOrientation(Orientation value, ValueSource source)
	{
		SetProperty(OrientationProperty, _orientation, value, _orientationSource, source);
	}

	float RangeBase::GetRelativeValue() const
	{
		if(_maxValue - _minValue < 1e-6f)
		{
			return 0;
		}

		return (_value - _minValue) / (_maxValue - _minValue);
	}

	void RangeBase::OnPropertyChanged(PropertyBase& property)
	{
		TemplatedUIElement::OnPropertyChanged(property);

		if(property == ValueProperty)
		{
			OnValueChanged(_value);
		}
		else if(property == MinValueProperty)
		{
			if(_minValue > _value)
			{
				SetValue(_minValue, _minValueSource);
			}
		}
		else if(property == MaxValueProperty)
		{
			if(_maxValue < _value)
			{
				SetValue(_maxValue, _maxValueSource);
			}
		}
		else if(property == OrientationProperty)
		{
			InvalidateRender();
		}		
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

