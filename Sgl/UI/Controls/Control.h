#pragma once

#include "../../Style/Font.h"
#include "../UIElement.h"

namespace Sgl::UI
{
	class Control: public UIElement
	{
	private:
		using base = UIElement;
	public:
		using FontFamilyRef = std::reference_wrapper<const FontFamily>;
		Event<MouseEventHandler> MouseDoubleClick;

		Thickness Padding;
		size_t FontSize = 14;
		Color FontColor = Colors::Black;
		FontStyle FontStyle = FontStyle::Normal;
		FontWeight FontWeight = FontWeight::Normal;
		FontFamilyRef FontFamily = FontFamily::Get("Segoe UI");
		TextDecoration TextDecoration = TextDecoration::None;
		Thickness BorderThickness = Thickness(1);
		Color BorderColor = Colors::Black;
	public:
		void ResetStyle() override
		{
			base::ResetStyle();

			Padding = {};
			TextDecoration = TextDecoration::None;
			FontSize = 14;
			FontFamily = FontFamily::Get("Segoe UI");
			FontStyle = FontStyle::Normal;
			FontWeight = FontWeight::Normal;
			FontColor = Colors::Black;
			BorderThickness = Thickness(1);
			BorderColor = Colors::Black;
		}
	protected:
		virtual void OnMouseDoubleClick(const MouseButtonEventArgs& e) {}
	};
}