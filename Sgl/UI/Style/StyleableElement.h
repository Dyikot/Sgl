#pragma once

#include <vector>
#include "StyleMap.h"

namespace Sgl
{
	class StyleableElement: public virtual IStyleProvider
	{
	public:
		StyleMap Styles;
		std::vector<std::string> Classes;
	protected:
		IStyleProvider* _stylingParent;
	private:
		bool _shouldRestyle = true;
		std::vector<shared_ptr<IStyle>> _styles;
	public:
		StyleableElement();
		StyleableElement(const StyleableElement&) = default;
		StyleableElement(StyleableElement&&) = default;
		virtual ~StyleableElement() = default;

		StyleMap& GetStyles() override { return Styles; }
		IStyleProvider* GetStylingParent() override { return _stylingParent; }

		virtual void ApplyStyle();
	private:
		void UpdateStyle();
		void GetStylesFrom(const StyleMap& styles);
	};
}