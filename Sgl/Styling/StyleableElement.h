#pragma once

#include <vector>
#include "StyleMap.h"
#include "../Data/ObservableObject.h"
#include "../Data/AttachableObject.h"
#include "../Base/Ref.h"

namespace Sgl
{
	class StyleableElement : public AttachableObject
	{
	public:
		StyleMap Styles;
	private:
		bool _isStyleValid = false;
		std::vector<std::string> _classList;
		std::vector<IStyle*> _styles;
		StyleableElement* _styleableParent = nullptr;
	public:
		StyleableElement() = default;
		StyleableElement(const StyleableElement& other);
		StyleableElement(StyleableElement&& other) noexcept;
		virtual ~StyleableElement() = default;

		void SetClasses(const std::string& classNames);
		void SetClasses(std::vector<std::string> classList);
		const std::vector<std::string>& GetClasses() const;

		virtual void SetParent(StyleableElement* parent);
		StyleableElement* GetStyleableParent() const { return _styleableParent; }

		bool IsStyleValid() const { return _isStyleValid; }

		virtual void ApplyStyle();
		void InvalidateStyle();
	private:
		void UpdateStyles();
		void GetStylesFrom(const StyleMap& styles);
	};
}