#pragma once

#include "Decorator.h"

namespace Sgl::UIElements
{
	class Border : public Decorator
	{
	public:
		Border();

		void SetBorderWidth(uint32_t value, ValueSource source = ValueSource::Local);
		uint32_t GetBorderWidth() const;

		void SetBorderColor(Color value, ValueSource source = ValueSource::Local);
		Color GetBorderColor() const;

		void Render(RenderContext context) override;

		static inline StyleableProperty BorderWidthProperty { &SetBorderWidth, &GetBorderWidth };
		static inline StyleableProperty BorderColorProperty { &SetBorderColor, &GetBorderColor };
	protected:
		void OnPropertyChanged(PropertyBase& property) override;
		FSize MeasureContent(FSize availableSize) override;
		void ArrangeContent(FRect rect) override;
	private:
		uint32_t _borderWidth = 1;
		Color _borderColor = Colors::Black;

		ValueSource _borderWidthSource {};
		ValueSource _borderColorSource {};
	};
}

namespace Sgl
{
	template<>
	class ResourceSetter<UIElements::Border, Color> final: public ISetter
	{
	public:
		using BorderColorProperty = decltype(UIElements::Border::BorderColorProperty);
	public:
		ResourceSetter(BorderColorProperty& property, std::string key);

		PropertyBase& GetProperty() const override;
		void Apply(Styleable& target, ValueSource valueSource) const override;
	private:
		BorderColorProperty& _property;
		std::string _key;
	};
}