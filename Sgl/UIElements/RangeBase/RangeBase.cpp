#include "RangeBase.h"

namespace Sgl::UIElements
{
	void RangeBase::SetValue(float value)
	{
		SetProperty(ValueProperty, _value, Clamp(value), _valueSource);
	}

	void RangeBase::SetMinValue(float value)
	{
		SetProperty(MinValueProperty, _minValue, value, _minValueSource);
	}

	void RangeBase::SetMaxValue(float value)
	{
		SetProperty(MaxValueProperty, _maxValue, value, _maxValueSource);
	}

	void RangeBase::SetOrientation(Orientation value)
	{
		SetProperty(OrientationProperty, _orientation, value, _orientationSource);
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
				SetProperty(ValueProperty, _value, _minValue);
			}
		}
		else if(property == MaxValueProperty)
		{
			if(_maxValue < _value)
			{
				SetProperty(ValueProperty, _value, _maxValue);
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

