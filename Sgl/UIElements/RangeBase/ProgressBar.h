#pragma once

#include "RangeBase.h"

namespace Sgl::UIElements
{
	class ProgressBar : public RangeBase
	{
	public:
		ProgressBar();
		ProgressBar(ProgressBar&& other) noexcept;

		void SetProgressColor(Color value, ValueSource source = ValueSource::Local);
		Color GetProgressColor() const { return _progressColor; }

		void Render(RenderContext context) override;

		static inline StyleableProperty ProgressColorProperty { &SetProgressColor, &GetProgressColor };
	private:
		Color _progressColor = Colors::Blue;
		ValueSource _progressColorSource {};
	};
}


