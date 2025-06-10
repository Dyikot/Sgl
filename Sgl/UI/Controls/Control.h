#pragma once

#include "../../Style/Font.h"
#include "../UIElement.h"

namespace Sgl::UI
{
	class Control: public UIElement
	{
	public:
		using FontFamilyRef = std::reference_wrapper<const FontFamily>;

	private:
		using base = UIElement;

		Layout* _layout = nullptr;

		Thickness _padding;
		size_t _fontSize = 14;
		Color _fontColor = Colors::Black;
		FontStyle _fontStyle = FontStyle::Normal;
		FontWeight _fontWeight = FontWeight::Normal;
		FontFamilyRef _fontFamily = FontFamily::Get("Segoe UI");
		TextDecoration _textDecoration = TextDecoration::None;
		Thickness _borderThickness = Thickness(1);
		Color _borderColor = Colors::Black;
	public:
		Control() = default;
		virtual ~Control() = default;

		void SetPadding(Thickness value) { _padding = value; }
		Thickness GetPadding() const { return _padding; }

		void SetFontSize(size_t value) { _fontSize = value; }
		size_t GetFontSize() const { return _fontSize; }

		void SetFontColor(Color value) { _fontColor = value; }
		Color GetFontColor() const { return _fontColor; }

		void SetFontStyle(FontStyle value) { _fontStyle = value; }
		FontStyle GetFontStyle() const { return _fontStyle; }

		void SetFontWeight(FontWeight value) { _fontWeight = value; }
		FontWeight GetFontWeight() const { return _fontWeight; }

		void SetFontFamily(const FontFamily& value) { _fontFamily = value; }
		const FontFamily& GetFontFamily() const { return _fontFamily; }

		void SetTextDecoration(TextDecoration value) { _textDecoration = value; }
		TextDecoration GetTextDecoration() const { return _textDecoration; }

		void SetBorderThickness(const Thickness& value) { _borderThickness = value; }
		Thickness GetBorderThickness() const { return _borderThickness; }

		void SetBorderColor(Color value) { _borderColor = value; }
		Color GetBorderColor() const { return _borderColor; }

		void ResetStyle() override
		{
			base::ResetStyle();

			_padding = {};
			_textDecoration = TextDecoration::None;
			_fontSize = 14;
			_fontFamily = FontFamily::Get("Segoe UI");
			_fontStyle = FontStyle::Normal;
			_fontWeight = FontWeight::Normal;
			_fontColor = Colors::Black;
			_borderThickness = Thickness(1);
			_borderColor = Colors::Black;
		}
	protected:
		void SetLayout(Layout& layout);

		virtual void OnMouseDoubleClick(const MouseButtonEventArgs& e) {}
	};
}