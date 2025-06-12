#pragma once

#include "../UIElement.h"
#include "../Layout/Layout.h"
#include "../../Visual/Font.h"

namespace Sgl::UI
{
	class Control: public UIElement
	{
	private:
		shared_ptr<Layout> _layout;

		Color _borderColor;
		Thickness _padding;
		Thickness _borderThickness;
		shared_ptr<Font> _font;
	public:
		Control();
		Control(const Control& other);
		Control(Control&& other) noexcept;
		virtual ~Control() = default;

		void SetLayout(shared_ptr<Layout> value);
		shared_ptr<Layout> GetLayout() const;

		void SetPadding(Thickness value) { _padding = value; }
		Thickness GetPadding() const { return _padding; }

		void SetBorderThickness(const Thickness& value) { _borderThickness = value; }
		Thickness GetBorderThickness() const { return _borderThickness; }

		void SetBorderColor(Color value) { _borderColor = value; }
		Color GetBorderColor() const { return _borderColor; }

		void SetFont(shared_ptr<Font> value) { _font = value; }
		auto GetFont() const { return _font; }

		void ResetToDefault() override;
	protected:
		virtual void OnMouseDoubleClick(const MouseButtonEventArgs& e) {}
	};
}