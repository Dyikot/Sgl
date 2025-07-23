#pragma once

#include <vector>
#include "StyleMap.h"

namespace Sgl
{
	class StyleableElement: public IStyleProvider
	{
	public:
		StyleMap Styles;
		std::vector<std::string> Classes;
	protected:
		IStyleProvider* _stylingParent;
	private:
		bool _isStyleValid;
		std::vector<std::shared_ptr<IStyle>> _styles;
	public:
		StyleableElement();
		StyleableElement(const StyleableElement& other);
		StyleableElement(StyleableElement&& other) noexcept;
		virtual ~StyleableElement() = default;

		bool IsStyleValid() const { return _isStyleValid; }
		StyleMap& GetStyles() override { return Styles; }
		IStyleProvider* GetStylingParent() override { return _stylingParent; }

		virtual void ApplyStyle();
		void InvalidateStyle();
	private:
		void UpdateStyle();
		void GetStylesFrom(const StyleMap& styles);
	};
}