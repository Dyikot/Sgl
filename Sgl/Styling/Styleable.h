#pragma once

#include "IStyleHost.h"
#include "StyleCollection.h"
#include "VisualState.h"
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
		VisualStateSet States;
		Event<StyleableElementEventHandler> AttachedToLogicalTree;
		Event<StyleableElementEventHandler> DetachedFromLogicalTree;

		void SetClasses(std::string_view classNames, char delimiter = ' ');
		void SetClasses(std::vector<std::string> classList);
		const std::vector<std::string>& GetClasses() const;

		StyleCollection& GetStyles() final;
		void MergeStylesTo(Styleable& element, Style& target) final;
		IStyleHost* GetStylingParent() const { return _stylingParent; }
		bool IsAttachedToLogicalTree() const noexcept { return _isAttachedToLogicalTree; }				
	protected:
		~Styleable() = default;
		virtual void SetParent(IStyleHost* parent);
		virtual void OnAttachedToLogicalTree();
		virtual void OnDetachedFromLogicalTree();
		void ApplyStyle();
		void ApplyStateStyle();
		void FetchAndApplyStyle();
	private:
		std::vector<std::string> _classes;
		Style _style;
		IStyleHost* _stylingParent = nullptr;
		bool _isAttachedToLogicalTree = false;
	};
}