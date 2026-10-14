#pragma once

#include "../UIElement.h"

namespace Sgl::UIElements
{
	class Track : public UIElement
	{
	public:
		Track();

		void SetFillRatio(float value);
		float GetFillRatio() const { return _fillRatio; }

		void SetOrientation(Orientation value);
		Orientation GetOrientation() const { return _orientation; }

		static inline StyleableProperty FillRatioProperty { &SetFillRatio, &GetFillRatio };
		static inline StyleableProperty OrientationProperty { &SetOrientation, &GetOrientation };
	protected:
		void OnPropertyChanged(PropertyBase& property) override;
		void ArrangeContent(FRect rect) override;
		Ref<Style> CreateDefaultStyle() const override;
	private:
		void BuildTemplate();
	private:
		float _fillRatio = 0;
		Orientation _orientation = Orientation::Horizontal;

		ValueSource _fillRatioSource {};
		ValueSource _orientationSource {};
	};

	class TrackStyle : public UIElementStyle
	{
	public:
		void OnBase(StyleContext& context) override;
	};

	class TrackProgressStyle : public UIElementStyle
	{
	public:
		void OnBase(StyleContext& context) override;
	};
}