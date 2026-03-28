#pragma once

#include "UIElementsCollection.h"

namespace Sgl
{
	class Panel : public UIElement
	{
	public:
		Panel();
		Panel(const Panel& other);
		Panel(Panel&& other) noexcept;

		UIElementsCollection Children;

		void SetVisualRoot(IVisualRoot* value) final;
		void Render(RenderContext context) final;
		void ApplyStyle() override;
	protected:
		void OnChildAdded(UIElement& child);
		void OnChildRemoving(UIElement& child);
		void OnAttachedToLogicalTree() override;
		void OnDetachedFromLogicalTree() override;
		void OnCursorChanged(const Cursor& cursor) override;
		void OnDataContextChanged(const Ref<INotifyPropertyChanged>& dataContext) override;
		void OnMouseMove(MouseMoveEventArgs e) override;
		void OnMouseDown(MouseButtonEventArgs e) override;
		void OnMouseUp(MouseButtonEventArgs e) override;
		void OnMouseLeave(MouseMoveEventArgs e) override;
	private:
		Ref<UIElement> _currentChild;
		Ref<UIElement> _mouseCapturedElement;

		friend class UIElementsCollection;
	};	

	struct Panel_FirstChild
	{
		StyleableElement& operator()(StyleableElement& target) const;
	};

	struct Panel_LastChild
	{
		StyleableElement& operator()(StyleableElement& target) const;
	};

	struct Panel_NthChild
	{
		size_t Index = 0;
		StyleableElement& operator()(StyleableElement& target) const;
	};
}