#pragma once

#include <set>
#include <unordered_map>
#include <memory>
#include "../UIElement.h"
#include "../../Data/Ranges.h"
#include "../../Data/Helpers.h"

namespace Sgl
{
	class Scene;
}

namespace Sgl::UI
{
	struct ZIndexComparer
	{
		bool operator()(const UIElement& left, const UIElement& right) const
		{
			return left.ZIndex < right.ZIndex;
		}
	};

	using UIElementCollection = std::multiset<Ref<UIElement>, ZIndexComparer>;

	class Layout: public UIElement
	{
	public:
		VisualElement& Parent;
	protected:
		UIElementCollection _children;
	private:
		using base = UIElement;

		UIElement* _hoverChild = nullptr;
		bool _isHover = false;
	public:
		Layout(VisualElement& parent);

		virtual void AddChild(UIElement& child);
		virtual void RemoveChild(UIElement& child);

		virtual void Arrange() = 0;
		virtual void Measure() = 0;

		void OnRender(RenderContext rc) const override
		{
			for(UIElement& child : _children)
			{
				child.OnRender(rc);
			}

			base::OnRender(rc);
		}

		const UIElementCollection& GetChildren() const { return _children; }
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

		void OnMouseDoubleClick(const MouseButtonEventArgs& e) override
		{
			base::OnMouseDoubleClick(e);

			for(UIElement& child : _children)
			{
				child.OnMouseDoubleClick(e);
			}
		}

		void OnMouseWheel(const MouseWheelEventArgs& e) override
		{
			base::OnMouseWheel(e);

			for(UIElement& child : _children)
			{
				child.OnMouseWheel(e);
			}
		}

		void OnKeyDown(const KeyEventArgs& e) override
		{
			base::OnKeyDown(e);

			for(UIElement& child : _children)
			{
				child.OnKeyDown(e);
			}
		}

		void OnKeyUp(const KeyEventArgs& e) override
		{
			base::OnKeyUp(e);

			for(UIElement& child : _children)
			{
				child.OnKeyUp(e);
			}
		}
	private:
		void OnRootLayoutMouseMove(const MouseButtonEventArgs& e);

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