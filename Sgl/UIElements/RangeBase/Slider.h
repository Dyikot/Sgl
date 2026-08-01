#pragma once

#include "RangeBase.h"
#include "Track.h"

namespace Sgl::UIElements
{
	class RepeatButton;

	class Slider : public RangeBase
	{
	public:
		Slider();
		Slider(Slider&&) noexcept = default;

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