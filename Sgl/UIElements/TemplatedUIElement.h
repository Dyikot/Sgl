#pragma once

#include "UIElement.h"

namespace Sgl
{
	class TemplatedUIElement : public UIElement
	{
	public:
		struct Template;
	public:
		TemplatedUIElement() = default;
		TemplatedUIElement(TemplatedUIElement&&) noexcept;

	protected:
		void SetTemplate(const Ref<UIElement>& template_);
		const Ref<UIElement>& GetTemplate() const { return _template; }

		FSize MeasureContent(FSize availableSize) override;
		void ArrangeContent(FRect rect) override;
	private:
		Ref<UIElement> _template;
	};

	struct TemplatedUIElement::Template
	{
		Styleable& operator()(Styleable& element) const;
	};
}