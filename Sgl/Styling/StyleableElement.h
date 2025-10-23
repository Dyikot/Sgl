#pragma once

#include <vector>
#include "StyleMap.h"
#include "ClassesCollections.h"
#include "../Data/ObservableObject.h"
#include "../Base/Ref.h"
#include "../Base/Collections/ResourcesMap.h"

namespace Sgl
{
	class StyleableElement: public ObservableObject, public IStyleProvider
	{
	public:
		StyleMap Styles;
		ClassesCollection Classes;
		ResourcesMap Resources;
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
		void UpdateStyles();
		void GetStylesFrom(const StyleMap& styles);
	};
}