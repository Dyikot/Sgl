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
		Style Class = { *this };
	private:
		UI::Layout* _layout = nullptr;
	public:
		Scene() = default;
		Scene(const Scene&) = delete;
		Scene(Scene&&) = delete;
		virtual ~Scene() = default;

		void OnRender(RenderContext rc) const override;
		virtual void OnUpdate(TimeSpan elapsed) = 0;
	protected:
		void SetLayout(UI::Layout& layout);

		virtual void OnStopped() {}
		virtual void OnResumed() {}
		virtual void OnCreated() {}
		virtual void OnDestroying() {}

		void OnMouseMove(const MouseButtonEventArgs& e) override
		{
			_layout->OnSceneLayoutMouseMove(e);
		}

		void OnMouseDown(const MouseButtonEventArgs& e) override
		{
			_layout->OnMouseDown(e);
		}

		void OnMouseUp(const MouseButtonEventArgs& e) override
		{
			_layout->OnMouseUp(e);
		}

		void OnMouseWheel(const MouseWheelEventArgs& e) override
		{
			_layout->OnMouseWheel(e);
		}

		void OnKeyDown(const KeyEventArgs& e) override
		{
			_layout->OnKeyDown(e);
		}

		void OnKeyUp(const KeyEventArgs& e) override
		{
			_layout->OnKeyUp(e);
		}

		friend class Application;
		friend class SceneManager;
	};
}