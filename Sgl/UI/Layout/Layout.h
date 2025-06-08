#pragma once

#include <set>
#include <unordered_map>
#include <memory>
#include "../UIElement.h"
#include "../../Data/Ranges/Ranges.h"
#include "../../Data/Size.h"
#include "../../Data/Ranges/SortedVector.h"

namespace Sgl
{
	class Scene;
}

namespace Sgl::UI
{
	class Layout: public UIElement
	{
	public:
		using UIElementCollection = SortedVector<std::shared_ptr<UIElement>, UIElementComparer>;

		VisualElement& Parent;
		UIElementCollection Children;
	private:
		using base = UIElement;

		std::shared_ptr<UIElement> _hoverChild;
		bool _isHover = false;
	public:
		Layout(VisualElement& parent);

		virtual void Arrange() = 0;
		virtual void Measure() = 0;

		void OnRender(RenderContext rc) const override;
	protected:
		static void SetAbsolutePosition(UIElement& element, FPoint position)
		{
			element._position = position;
		}

		static FPoint GetAbsolutePosition(UIElement& element)
		{
			return element._position;
		}

		void OnMouseMove(const MouseButtonEventArgs& e) override;

		void OnMouseDown(const MouseButtonEventArgs& e) override
		{
			base::OnMouseDown(e);

			if(_hoverChild)
			{
				_hoverChild->OnMouseDown(e);
			}
		}

		void OnMouseUp(const MouseButtonEventArgs& e) override
		{
			base::OnMouseUp(e);

			if(_hoverChild)
			{
				_hoverChild->OnMouseUp(e);
			}
		}

		void OnMouseWheel(const MouseWheelEventArgs& e) override
		{
			for(auto& child : Children)
			{
				child->OnMouseWheel(e);
			}
		}

		void OnKeyDown(const KeyEventArgs& e) override
		{
			for(auto& child : Children)
			{
				child->OnKeyDown(e);
			}
		}

		void OnKeyUp(const KeyEventArgs& e) override
		{
			for(auto& child : Children)
			{
				child->OnKeyUp(e);
			}
		}
	private:
		void OnSceneLayoutMouseMove(const MouseButtonEventArgs& e);

		FPoint GetPosition() const
		{
			return FPoint
			{
				.x = static_cast<float>(std::max(Margin.Left, Margin.Right)),
				.y = static_cast<float>(std::max(Margin.Top, Margin.Bottom))
			};
		}

		friend class Sgl::Scene;
	};
}