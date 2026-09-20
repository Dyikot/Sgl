#pragma once

#include "Panel.h"

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
		
		static void SetDock(const Ref<UIElement>& element, Dock value);
		static Dock GetDock(const Ref<UIElement>& element);

		void SetLastChildFill(bool value, ValueSource source = ValueSource::Local);
		bool GetLastChildFill() const { return _lastChildFill; }

		static inline StyleableProperty LastChildFillProperty { &SetLastChildFill, &GetLastChildFill };
	protected:
		void OnPropertyChanged(PropertyBase& property) override;
		FSize MeasureContent(FSize availableSize) override;
		void ArrangeContent(FRect rect) override;
	private:
		bool _lastChildFill = false;

		ValueSource _lastChildFillSource {};
	};
}