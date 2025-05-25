#pragma once

#include <set>
#include <unordered_map>
#include "UIElement.h"
#include "../Data/Ranges.h"
#include "../Data/Helpers.h"

namespace Sgl
{
	class Scene;
}

namespace Sgl::UI
{
	class Layout: public UIElement
	{
	public:
		struct UIElementContext
		{
			FPoint Position;
			UIElement& Element;
		};

		struct ZIndexComparer
		{
			bool operator()(const UIElementContext& left, const UIElementContext& right) const
			{
				return left.Element.ZIndex < right.Element.ZIndex;
			}
		};

		VisualElement& Parent;
	protected:
		std::multiset<UIElementContext, ZIndexComparer> _children;
	private:
		using base = UIElement;

		const UIElementContext* _hoverElementContext = nullptr;
	public:
		Layout(VisualElement& parent);

		void OnRender(RenderContext rc) const override
		{
			for(auto& context : _children)
			{
				context.Element.OnRender(rc);
			}

			base::OnRender(rc);
		}

		auto GetChildren() const
		{
			return _children | Ranges::Select(
				[](const UIElementContext& context) -> Ref<UIElement>
				{
					return context.Element; 
				}
			);
		}
	protected:
		void OnMouseMove(const MouseButtonEventArgs& e) override;

		void OnMouseEnter(const MouseButtonEventArgs& e) override
		{

		}

		void OnMouseLeave(const MouseButtonEventArgs& e) override
		{

		}

		void OnMouseDown(const MouseButtonEventArgs& e) override
		{
			if(_hoverElementContext)
			{
				_hoverElementContext->Element.OnMouseDown(e);
			}
		}

		void OnMouseUp(const MouseButtonEventArgs& e) override
		{
			if(_hoverElementContext)
			{
				_hoverElementContext->Element.OnMouseUp(e);
			}
		}

		void OnMouseDoubleClick(const MouseButtonEventArgs& e) override
		{
			for(auto& child : _children)
			{
				child.Element.OnMouseDoubleClick(e);
			}
		}

		void OnMouseWheel(const MouseWheelEventArgs& e) override
		{
			for(auto& child : _children)
			{
				child.Element.OnMouseWheel(e);
			}
		}

		void OnKeyDown(const KeyEventArgs& e) override
		{
			for(auto& child : _children)
			{
				child.Element.OnKeyDown(e);
			}
		}

		void OnKeyUp(const KeyEventArgs& e) override
		{
			for(auto& child : _children)
			{
				child.Element.OnKeyUp(e);
			}
		}

		friend class Sgl::Scene;
	};
}