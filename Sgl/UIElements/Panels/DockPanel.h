#pragma once

#include "Panel.h"
#include "../../Data/LayoutProperty.h"

namespace Sgl::UIElements
{
	//! @brief Specifies the dock position of an element within DockPanel
	enum class Dock
	{
		Left,   // Docks the element to the left edge
		Right,  // Docks the element to the right edge
		Top,    // Docks the element to the top edge
		Bottom  // Docks the element to the bottom edge
	};

	class DockPanel : public Panel
	{
	public:
		DockPanel();
		DockPanel(DockPanel&&) noexcept = default;
		
		static void SetDock(const Ref<UIElement>& element, Dock dock);
		static Dock GetDock(const Ref<UIElement>& element);

		void SetLastChildFill(bool value, ValueSource source = ValueSource::Local);
		bool GetLastChildFill() const { return _lastChildFill; }

		static LayoutProperty<Dock> DockProperty;
		static inline StyleableProperty LastChildFillProperty { &SetLastChildFill, &GetLastChildFill };
	protected:
		FSize MeasureContent(FSize availableSize) override;
		void ArrangeContent(FRect rect) override;
	private:
		bool _lastChildFill = false;

		ValueSource _lastChildFillSource {};
	};
}