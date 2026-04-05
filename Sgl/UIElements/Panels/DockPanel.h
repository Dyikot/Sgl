#pragma once

#include "Panel.h"
#include "../../Data/LayoutProperty.h"

namespace Sgl::UIElements
{
	enum class Dock
	{
		Left, 
		Right, 
		Top, 
		Bottom
	};

	class DockPanel : public Panel
	{
	public:
		DockPanel() = default;
		DockPanel(const DockPanel&) = delete;
		DockPanel(DockPanel&&) noexcept = default;
		
		static void SetDock(const Ref<UIElement>& element, Dock dock);
		static Dock GetDock(const Ref<UIElement>& element) { return element->GetLayoutContext<Dock>(); }

		void SetLastChildFill(bool value, ValueSource source = ValueSource::Local);
		bool GetLastChildFill() const { return _lastChildFill; }

		static LayoutProperty<Dock> DockProperty;
		static inline StyleableProperty LastChildFillProperty { &SetLastChildFill, &GetLastChildFill };
	protected:
		FSize MeasureContent(FSize avaliableSize) override;
		void ArrangeContent(FRect rect) override;
	private:
		bool _lastChildFill = false;

		ValueSource _lastChildFillSource {};
	};
}