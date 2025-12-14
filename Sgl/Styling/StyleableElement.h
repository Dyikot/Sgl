#pragma once

#include <vector>

#include "IStyleHost.h"
#include "../Data/ObservableObject.h"
#include "../Data/AttachableObject.h"
#include "../Base/Ref.h"
#include "../Base/Event.h"

namespace Sgl
{
	class StyleableElement : public AttachableObject, public IStyleHost
	{
	private:
		using StyleableElementEventHandler = EventHandler<StyleableElement>;
	public:
		StyleMap Styles;
		Event<StyleableElementEventHandler> AttachedToElementsTree;
		Event<StyleableElementEventHandler> DetachedFromElementsTree;
	private:
		std::vector<std::string> _classList;
		std::vector<IStyle*> _styles;
		IStyleHost* _stylingParent = nullptr;
		IStyleRoot* _stylingRoot = nullptr;
	public:
		StyleableElement() = default;
		StyleableElement(const StyleableElement& other);
		StyleableElement(StyleableElement&& other) noexcept;
		virtual ~StyleableElement() = default;

		void SetClasses(const std::string& classNames);
		void SetClasses(std::vector<std::string> classList);
		const std::vector<std::string>& GetClasses() const;

		virtual void SetParent(IStyleHost* parent);
		StyleMap& GetStyles() final { return Styles; }
		IStyleHost* GetStylingParent() final { return _stylingParent; }

		bool IsAttachedToLogicalTree() const { return _stylingRoot != nullptr; }

		virtual void ApplyStyle();
	protected:
		virtual void OnAttachedToLogicalTree();
		virtual void OnDetachedFromLogicalTree();
	private:
		bool UpdateStyle();
		void OnStyleClassesChanged();
		void GetStylesFrom(const StyleMap& styles);
	};
}