#pragma once

#include <vector>
#include "StyleMap.h"
#include "ClassesCollections.h"
#include "../Binding/BindableObject.h"
#include "../../Base/Collections/ResourcesMap.h"

namespace Sgl
{
	class StyleableElement: public BindableObject, public IStyleProvider
	{
	public:
		StyleMap Styles;
		ClassesCollection Classes;
		ResourcesMap Resources;
		Shared<void> DataContext;
	protected:
		IStyleProvider* _stylingParent {};
	private:
		bool _isStyleValid = false;
		std::vector<Ref<IStyle>> _styles;
	public:
		StyleableElement() = default;
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