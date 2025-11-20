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
	protected:
		StyleableElement* _parent = nullptr;
	private:
		bool _isStyleValid = false;
		std::vector<std::string> _classList;
		std::vector<IStyle*> _styles;
	public:
		StyleableElement() = default;
		StyleableElement(const StyleableElement& other);
		StyleableElement(StyleableElement&& other) noexcept;
		virtual ~StyleableElement() = default;

		void SetClasses(const std::string& classNames);
		void SetClasses(std::vector<std::string> classList);
		const std::vector<std::string>& GetClasses() const;

		bool IsStyleValid() const { return _isStyleValid; }
		StyleableElement* GetParent() const { return _parent; }

		virtual void ApplyStyle();
		void InvalidateStyle();
	private:
		void UpdateStyles();
		void GetStylesFrom(const StyleMap& styles);
	};
}