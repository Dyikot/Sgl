#pragma once

#include "Style.h"
#include "../Data/Bindable.h"

namespace Sgl
{
	class Styleable : public Bindable
	{
	public:
		using StyleableElementEventHandler = EventHandler<Styleable>;
	public:
		Styleable();

		VisualStateSet States;
		Event<StyleableElementEventHandler> AttachedToLogicalTree;
		Event<StyleableElementEventHandler> DetachedFromLogicalTree;

		void SetName(const std::string& value);
		const std::string& GetName() const { return _name; }

		void SetStyle(Ref<Style> style);

		Styleable* GetStylingParent() const { return _stylingParent; }
		bool IsAttachedToLogicalTree() const noexcept { return _isAttachedToLogicalTree; }				
	protected:
		~Styleable() = default;
		virtual void SetParent(Styleable* parent);
		virtual void OnAttachedToLogicalTree();
		virtual void OnDetachedFromLogicalTree();
		virtual Ref<Style> CreateDefaultStyle() const;
		void ApplyStyle();
		void ApplyStateStyle();
	private:
		std::vector<std::string> _classes;
		Ref<Style> _style;
		std::string _name;
		Styleable* _stylingParent = nullptr;
		bool _isAttachedToLogicalTree = false;
	};
}