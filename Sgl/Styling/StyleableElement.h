#pragma once

#include <vector>

#include "IStyleHost.h"
#include "PseudoClass.h"
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
		StyleableElement() = default;
		StyleableElement(const StyleableElement& other);
		StyleableElement(StyleableElement&& other) noexcept;
		virtual ~StyleableElement() = default;

		std::string Name;
		StyleCollection Styles;
		PseudoClassesSet PseudoClasses;
		Event<StyleableElementEventHandler> AttachedToLogicalTree;
		Event<StyleableElementEventHandler> DetachedFromLogicalTree;

		void SetClasses(const std::string& classNames);
		void SetClasses(std::vector<std::string> classList);
		const std::vector<std::string>& GetClasses() const;

		virtual void SetParent(IStyleHost* parent);
		StyleCollection& GetStyles() final { return Styles; }

		IStyleHost* GetStylingParent() final { return _stylingParent; }

		bool IsAttachedToLogicalTree() const noexcept { return _isAttachedToLogicalTree; }

		virtual void ApplyStyle();
	protected:
		virtual void OnAttachedToLogicalTree();
		virtual void OnDetachedFromLogicalTree();
	private:
		bool FetchStyles();
		void FetchStylesFrom(const StyleCollection& styles);
		void OnStyleClassesChanged();
	private:
		std::vector<std::string> _classList;
		std::vector<const Style*> _styles;
		IStyleHost* _stylingParent = nullptr;
		bool _isAttachedToLogicalTree = false;
	};
}