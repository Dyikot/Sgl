#pragma once

#include <vector>
#include <stack>
#include <string_view>

#include "IStyleHost.h"
#include "PseudoClass.h"
#include "../Data/BindableObject.h"
#include "../Base/Ref.h"
#include "../Base/Event.h"

namespace Sgl
{
	class StyleableElement : public BindableObject, public IStyleHost
	{
	public:
		struct BaseState
		{
			PropertyBase& Property;
			Action<BindableObject&> Restore;
		};
	private:
		using StyleableElementEventHandler = EventHandler<StyleableElement>;
	public:
		StyleableElement();
		StyleableElement(const StyleableElement& other);
		StyleableElement(StyleableElement&& other) noexcept;
		~StyleableElement() = default;

		std::string Name;
		StyleCollection Styles;
		PseudoClassesSet PseudoClasses;
		Event<StyleableElementEventHandler> AttachedToLogicalTree;
		Event<StyleableElementEventHandler> DetachedFromLogicalTree;

		void SetClasses(std::string_view classNames);
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
		void AddBaseState(PropertyBase& property, Action<BindableObject&> restoreState) final;
		bool HasBaseState(PropertyBase& property) const final;
		void RestoreState();
		bool FetchStyles();
		void FetchStylesFrom(const StyleCollection& styles);
		void OnStyleClassesChanged();
	private:
		std::vector<std::string> _classList;
		std::vector<const Style*> _styles;
		std::vector<const Style*> _stateStyles;
		std::vector<BaseState> _baseStates;
		IStyleHost* _stylingParent = nullptr;
		bool _isAttachedToLogicalTree = false;
	};
}