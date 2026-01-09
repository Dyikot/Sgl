#pragma once

#include <vector>

#include "IStyleHost.h"
#include "../Data/BindableObject.h"
#include "../Base/Ref.h"
#include "../Base/Event.h"

namespace Sgl
{
	class StyleableElement : public BindableObject, public IStyleHost
	{
	private:
		using StyleableElementEventHandler = EventHandler<StyleableElement>;
	public:
		StyleCollection Styles;
		Event<StyleableElementEventHandler> AttachedToElementsTree;
		Event<StyleableElementEventHandler> DetachedFromElementsTree;
	private:
		std::vector<std::string> _classList;
		std::vector<const Style*> _styles;
		IStyleHost* _stylingParent = nullptr;
		IStyleHost* _stylingRoot = nullptr;
	public:
		StyleableElement() = default;
		StyleableElement(const StyleableElement& other);
		StyleableElement(StyleableElement&& other) noexcept;
		virtual ~StyleableElement() = default;

		void SetClasses(const std::string& classNames);
		void SetClasses(std::vector<std::string> classList);
		const std::vector<std::string>& GetClasses() const;

		virtual void SetParent(IStyleHost* parent);
		StyleCollection& GetStyles() final { return Styles; }

		virtual void SetStylingRoot(IStyleHost* value);
		IStyleHost* GetStylingRoot() final { return _stylingRoot; }

		IStyleHost* GetStylingParent() final { return _stylingParent; }

		bool IsAttachedToLogicalTree() const { return _stylingRoot != nullptr; }

		virtual void ApplyStyle();
	protected:
		virtual void OnAttachedToLogicalTree();
		virtual void OnDetachedFromLogicalTree();
	private:
		bool UpdateStyle();
		void OnStyleClassesChanged();
		void GetStylesFrom(const StyleCollection& styles);
	};
}