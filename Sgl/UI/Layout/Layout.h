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
	struct ZIndexComparer
	{
		bool operator()(const std::shared_ptr<UIElement>& left, 
						const std::shared_ptr<UIElement>& right) const
		{
			return left->ZIndex < right->ZIndex;
		}
	};

	class Layout: public UIElement
	{
	public:
		using UIElementCollection = SortedVector<std::shared_ptr<UIElement>, ZIndexComparer>;

		VisualElement& Parent;
	protected:
		UIElementCollection _children;
	private:
		using base = UIElement;

		std::shared_ptr<UIElement> _hoverChild;
		bool _isHover = false;
	public:
		Layout(VisualElement& parent);

		virtual void Arrange() = 0;
		virtual void Measure() = 0;

		template<std::derived_from<UIElement> TUIElement, typename... TArgs>
		auto Add(TArgs&&... args)
		{
			auto element = std::make_shared<TUIElement>(std::forward<TArgs>(args)...);
			_children.Add(element);
			return ElementConfigurer(*element);
		}

		void OnRender(RenderContext rc) const override
		{
			for(auto& child : _children)
			{
				child->OnRender(rc);
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

		void OnMouseWheel(const MouseWheelEventArgs& e) override
		{
			base::OnMouseWheel(e);

			for(auto& child : _children)
			{
				child->OnMouseWheel(e);
			}
		}

		void OnKeyDown(const KeyEventArgs& e) override
		{
			base::OnKeyDown(e);

			for(auto& child : _children)
			{
				child->OnKeyDown(e);
			}
		}

		void OnKeyUp(const KeyEventArgs& e) override
		{
			base::OnKeyUp(e);

			for(auto& child : _children)
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