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

		std::span<const Ref<UIElement>> GetVisualChildren() const final;
		FSize MeasureContent(FSize availableSize) override;
		void ArrangeContent(FRect rect) override;
	private:
		Ref<UIElement> _template;
	};

	struct TemplatedUIElement::Template
	{
		StyleableElement& operator()(StyleableElement& element) const;
	};
}