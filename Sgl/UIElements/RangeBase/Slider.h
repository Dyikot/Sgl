#pragma once

#include "RangeBase.h"

namespace Sgl::UIElements
{
	class RepeatButton;
	class Track;

	class Slider : public RangeBase
	{
	public:
		Slider();
		Slider(Slider&& other) noexcept;

	protected:
		void OnValueChanged(float value) override;
		void OnPropertyChanged(PropertyBase& property) override;
	private:
		void BuildTemplate();
	private:
		Ref<RepeatButton> _button;
		Ref<Track> _track;
	};
}