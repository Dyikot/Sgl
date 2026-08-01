#pragma once

#include "../TemplatedUIElement.h"

namespace Sgl::UIElements
{
	class Track : public TemplatedUIElement
	{
	public:
		Track();
		Track(Track&& other) noexcept;

		void SetFillRatio(float value, ValueSource source = ValueSource::Local);
		float GetFillRatio() const { return _fillRatio; }

		void SetOrientation(Orientation value, ValueSource source = ValueSource::Local);
		Orientation GetOrientation() const { return _orientation; }

		static inline StyleableProperty FillRatioProperty { &SetFillRatio, &GetFillRatio };
		static inline StyleableProperty OrientationProperty { &SetOrientation, &GetOrientation };
	protected:
		void ArrangeContent(FRect rect) override;
	private:
		void BuildTemplate();
	private:
		float _fillRatio = 0;
		Orientation _orientation = Orientation::Horizontal;

		ValueSource _fillRatioSource {};
		ValueSource _orientationSource {};
	};
}