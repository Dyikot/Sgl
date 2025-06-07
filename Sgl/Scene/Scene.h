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
		using SceneEventHandler = EventHandler<Scene, EventArgs>;

		Style Class;
		std::unique_ptr<UI::Layout> Layout;

		Event<KeyEventHandler> KeyUp;
		Event<KeyEventHandler> KeyDown;
		Event<SceneEventHandler> Stopped;
		Event<SceneEventHandler> Resumed;
		Event<SceneEventHandler> Created;
		Event<SceneEventHandler> Destroying;
	private:
		using base = VisualElement;
	public:
		Scene(std::unique_ptr<UI::Layout> layout);
		Scene(Func<std::unique_ptr<UI::Layout>> layoutFactory);
		Scene(const Scene&) = delete;
		Scene(Scene&&) = delete;
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
			Layout->OnKeyDown(e);
			KeyDown.TryRaise(*this, e);
		}

		void OnKeyUp(const KeyEventArgs& e) override
		{
			Layout->OnKeyUp(e);
			KeyUp.TryRaise(*this, e);
		}
	private:
		void OnStopped(const EventArgs& e)
		{
			Stopped.TryRaise(*this, e);
		}

		void OnResumed(const EventArgs& e)
		{
			Resumed.TryRaise(*this, e);
		}

		void OnCreated(const EventArgs& e)
		{
			Created.TryRaise(*this, e);
		}

		void OnDestroying(const EventArgs& e)
		{
			Destroying.TryRaise(*this, e);
		}

		friend class Application;
		friend class SceneManager;
	};
}