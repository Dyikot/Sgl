#pragma once

#include "IStyleHost.h"
#include "StyleCollection.h"
#include "PseudoClass.h"
#include "../Data/Bindable.h"

namespace Sgl
{
	class Styleable : public Bindable, public IStyleHost
	{
	public:
		using StyleableElementEventHandler = EventHandler<Styleable>;
	public:
		Styleable();

		std::string Name;
		StyleCollection Styles;
		PseudoClassesSet PseudoClasses;
		Event<StyleableElementEventHandler> AttachedToLogicalTree;
		Event<StyleableElementEventHandler> DetachedFromLogicalTree;

		void SetClasses(std::string_view classNames);
		void SetClasses(std::vector<std::string> classList);
		const std::vector<std::string>& GetClasses() const;

		StyleCollection& GetStyles() final;
		void WithStyles(const Action<const StyleCollection&>& action) const final;
		IStyleHost* GetStylingParent() const { return _stylingParent; }
		bool IsAttachedToLogicalTree() const noexcept { return _isAttachedToLogicalTree; }
				
	protected:
		~Styleable() = default;
		virtual void SetParent(IStyleHost* parent);
		virtual void OnAttachedToLogicalTree();
		virtual void OnDetachedFromLogicalTree();
		void FetchAndApplyStyle();
	private:
		void FetchAndApplyStylesFrom(const StyleCollection& styles);
		void ApplyStateStyle(const std::vector<const Style*>& styles);
		void RestoreBaseState();
		std::vector<const Style*> MatchStateStyles();
	private:
		std::vector<std::string> _classes;
		std::vector<const Style*> _stateStyles;
		std::vector<std::unique_ptr<ISavedValue>> _savedValues;
		IStyleHost* _stylingParent = nullptr;
		bool _isAttachedToLogicalTree = false;
	};
}