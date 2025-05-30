#pragma once

#include "../UI/Layout/Layout.h"
#include "../Tools/Time/TimeSpan.h"

namespace Sgl
{	
	class Scene: 
		public VisualElement,
		public IKeyEventsListener,
		public IMouseEventsListener	
	{
	public:
		Style Class;
		std::unique_ptr<UI::Layout> Layout;

		Event<KeyEventHandler> KeyUp;
		Event<KeyEventHandler> KeyDown;
	public:
		Scene(std::unique_ptr<UI::Layout> layout);
		virtual ~Scene() = default;

		void OnRender(RenderContext rc) const override;
		virtual void OnUpdate(TimeSpan elapsed) = 0;
	protected:
		void OnMouseMove(const MouseButtonEventArgs& e) override
		{
			Layout->OnSceneLayoutMouseMove(e);
		}

		void OnMouseDown(const MouseButtonEventArgs& e) override
		{
			Layout->OnMouseDown(e);
		}

		void OnMouseUp(const MouseButtonEventArgs& e) override
		{
			Layout->OnMouseUp(e);
		}

		void OnMouseWheel(const MouseWheelEventArgs& e) override
		{
			Layout->OnMouseWheel(e);
		}

		void OnKeyDown(const KeyEventArgs& e) override
		{
			KeyDown.TryRaise(*this, e);
			Layout->OnKeyDown(e);
		}

		void OnKeyUp(const KeyEventArgs& e) override
		{
			KeyUp.TryRaise(*this, e);
			Layout->OnKeyUp(e);
		}

		friend class Application;
	};
}