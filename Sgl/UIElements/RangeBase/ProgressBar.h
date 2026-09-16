#pragma once

#include "RangeBase.h"

namespace Sgl::UIElements
{
	class Track;

	class ProgressBar : public RangeBase
	{
	public:
		ProgressBar();
	protected:
		void OnValueChanged(float value) override;
		void OnPropertyChanged(PropertyBase& property) override;
	private:
		void BuildTemplate();
	private:
		Ref<Track> _track;
	};
}


