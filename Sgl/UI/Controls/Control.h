#pragma once

#include "../UIElement.h"
#include "../../Visual/Font.h"

namespace Sgl::UI
{
	class Control: public UIElement
	{
	private:
		using base = UIElement;

		Layout* _layout = nullptr;

		Color _borderColor;
		Thickness _padding;
		Thickness _borderThickness;
		std::shared_ptr<Font> _font;
	public:
		Control();
		Control(const Control& other);
		Control(Control&& other) noexcept;
		virtual ~Control() = default;

		void SetPadding(Thickness value) { _padding = value; }
		Thickness GetPadding() const { return _padding; }

		void SetBorderThickness(const Thickness& value) { _borderThickness = value; }
		Thickness GetBorderThickness() const { return _borderThickness; }

		void SetBorderColor(Color value) { _borderColor = value; }
		Color GetBorderColor() const { return _borderColor; }

		void SetFont(std::shared_ptr<Font> value) { _font = value; }
		auto GetFont() const { return _font; }

		void ResetToDefault() override;
	protected:
		void SetLayout(Layout& layout);

		virtual void OnMouseDoubleClick(const MouseButtonEventArgs& e) {}
	};
}