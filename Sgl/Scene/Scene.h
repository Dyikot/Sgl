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
	private:
		shared_ptr<UI::Layout> _layout;
	public:
		Scene();
		Scene(const Scene&) = delete;
		Scene(Scene&&) = delete;
		virtual ~Scene() = default;

		void SetLayout(shared_ptr<UI::Layout> value);
		shared_ptr<UI::Layout> GetLayout() const;

		void OnRender(RenderContext rc) const override;
		virtual void OnUpdate(TimeSpan elapsed) = 0;
	protected:
		virtual void OnStopped() {}
		virtual void OnResumed() {}
		virtual void OnCreated() {}
		virtual void OnDestroying() {}

		void OnMouseMove(const MouseButtonEventArgs& e) override;
		void OnMouseDown(const MouseButtonEventArgs& e) override;
		void OnMouseUp(const MouseButtonEventArgs& e) override;
		void OnMouseWheel(const MouseWheelEventArgs& e) override;
		void OnKeyDown(const KeyEventArgs& e) override;
		void OnKeyUp(const KeyEventArgs& e) override;

		friend class Application;
		friend class SceneManager;
	};
}