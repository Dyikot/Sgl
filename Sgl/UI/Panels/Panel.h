#pragma once

#include "../Object.h"
#include <set>

namespace Sgl
{
	class Control;

	class Panel: public Object
	{
	public:
		Control* MouseOverControl = nullptr;
		std::set<Control*> Children;
	protected:
		Paint _background = &Colors::Black;
		size_t _zindex = 1;
	public:
		Panel() = default;
		Panel(const Style& style);

		void SetBackground(const Paint& value) { _background = value; }
		void SetZIndex(size_t value) { _zindex = value; }

		const Paint& GetBackground() const { return _background; }
		size_t GetZIndex() const { return _zindex; }

		bool TryRaiseMouseMoveEvents(const MouseButtonEventArgs& e);
		void RaiseMouseDownEvents(const MouseButtonEventArgs& e);
		void RaiseMouseUpEvents(const MouseButtonEventArgs& e);
	private:
	};

	struct PanelComparer
	{
		bool operator()(const Panel* left, const Panel* right) const
		{
			left->GetZIndex() < right->GetZIndex();
		}
	};
}