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
		bool operator()(const std::shared_ptr<UIElement>& left, 
						const std::shared_ptr<UIElement>& right) const
		{
			return left->ZIndex < right->ZIndex;
		}
	};

	using UIElementCollection = std::multiset<std::shared_ptr<UIElement>, ZIndexComparer>;

	class Layout: public UIElement
	{
	public:
		template<typename TUIElement>
		class AddChildContext
		{
		public:
			std::shared_ptr<TUIElement> Element;
		public:
			AddChildContext(std::shared_ptr<TUIElement> element):
				Element(std::move(element))
			{}

			template<Style::Setter... Setters>
			AddChildContext& Class()
			{
				Element->ClassStyle.Use<Setters...>();
				return *this;
			}
		};

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
			return AddChildContext(std::make_shared<TUIElement>(std::forward<TArgs>(args)...));
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

		void OnMouseDoubleClick(const MouseButtonEventArgs& e) override
		{
			base::OnMouseDoubleClick(e);

			for(auto& child : _children)
			{
				child->OnMouseDoubleClick(e);
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