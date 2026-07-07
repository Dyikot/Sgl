#pragma once

#include <vector>
#include <string_view>

#include "IStyleHost.h"
#include "PseudoClass.h"
#include "../Data/BindableObject.h"

namespace Sgl
{
	class StyleableElement : public BindableObject, public IStyleHost
	{
	public:
		using StyleableElementEventHandler = EventHandler<StyleableElement>;
	public:
		StyleableElement();
		StyleableElement(StyleableElement&& other) noexcept;

		std::string Name;
		StyleCollection Styles;
		PseudoClassesSet PseudoClasses;
		Event<StyleableElementEventHandler> AttachedToLogicalTree;
		Event<StyleableElementEventHandler> DetachedFromLogicalTree;

		void SetClasses(std::string_view classNames);
		void SetClasses(std::vector<std::string> classList);
		const std::vector<std::string>& GetClasses() const;

		StyleCollection& GetStyles() final { return Styles; }
		IStyleHost* GetStylingParent() final { return _stylingParent; }
		bool IsAttachedToLogicalTree() const noexcept { return _isAttachedToLogicalTree; }

		void ApplyStyle();
	protected:
		virtual void SetParent(IStyleHost* parent);
		virtual void OnAttachedToLogicalTree();
		virtual void OnDetachedFromLogicalTree();
		void OnDataContextChanged(const Ref<INotifyPropertyChanged>& dataContext) final;
		void AddLogicalChild(StyleableElement* child);
		void RemoveLogicalChild(StyleableElement* child);
		const std::vector<StyleableElement*>& GetLogicalChildren() const { return _logicalChildren; }
	private:
		bool FetchStyles();
		void FetchStylesFrom(const StyleCollection& styles);
		void OnStyleClassesChanged();
		void ApplyStateStyle();
		void SaveBaseState();
		void RestoreBaseState();
		bool MatchStateStyles();
		void ClearAndRestoreBaseState();
	private:
		struct RestoreAction
		{
			Action<> Restore;
			StyleableElement* Target;
			StyleableElementEventHandler DetachedHandler;
		};

		std::vector<StyleableElement*> _logicalChildren;
		std::vector<std::string> _classList;
		std::vector<const Style*> _styles;
		std::vector<const Style*> _stateStyles;
		std::vector<const Style*> _matchingStateStyles;
		std::vector<RestoreAction> _restoreStateActions;
		IStyleHost* _stylingParent = nullptr;
		bool _isAttachedToLogicalTree = false;

		friend class ClearBaseStateHandler;
	};
}