#pragma once

#include "UIElement.h"

namespace Sgl
{
	class TemplatedUIElement : public UIElement
	{
	public:
		TemplatedUIElement() = default;
		TemplatedUIElement(TemplatedUIElement&&) noexcept;

	protected:
		void SetTemplate(const Ref<UIElement>& atemplate);
		const Ref<UIElement>& GetTemplate() const { return _template; }

		std::span<const Ref<UIElement>> GetChildren() const final;
		FSize MeasureContent(FSize availableSize) override;
		void ArrangeContent(FRect rect) override;
	private:
		Ref<UIElement> _template;
	};
}