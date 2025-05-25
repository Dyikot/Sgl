#pragma once

#include <stack>
#include <queue>
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

		Event<KeyEventHandler> KeyUp;
		Event<KeyEventHandler> KeyDown;
	private:
		UI::Layout& _layout;
	public:
		Scene(UI::Layout& layout);
		virtual ~Scene() = default;

		void OnRender(RenderContext rc) const final;
		virtual void OnUpdate(TimeSpan elapsed) {}
	protected:
		void OnMouseMove(const MouseButtonEventArgs& e) override
		{
			_layout.OnRootLayoutMouseMove(e);
		}

		void OnMouseDown(const MouseButtonEventArgs& e) override
		{
			_layout.OnMouseDown(e);
		}

		void OnMouseUp(const MouseButtonEventArgs& e) override
		{
			_layout.OnMouseUp(e);
		}

		void OnMouseDoubleClick(const MouseButtonEventArgs& e) override
		{
			_layout.OnMouseDoubleClick(e);
		}

		void OnMouseWheel(const MouseWheelEventArgs& e) override
		{
			_layout.OnMouseWheel(e);
		}

		void OnKeyDown(const KeyEventArgs& e) override
		{
			KeyDown.TryRaise(*this, e);
			_layout.OnKeyDown(e);
		}

		void OnKeyUp(const KeyEventArgs& e) override
		{
			KeyUp.TryRaise(*this, e);
			_layout.OnKeyUp(e);
		}

		friend class Application;
	};

	class SceneManager
	{
	private:
		using SceneFactory = Func<std::shared_ptr<Scene>>;

		std::stack<std::shared_ptr<Scene>> _scenes;
		std::queue<SceneFactory> _sceneFactoriesQueue;
		size_t _popScenes = 0;
	public:
		template<std::derived_from<Scene> TScene>
		void Push() noexcept
		{
			_sceneFactoriesQueue.push(&std::make_shared<TScene>);
		}

		void Push(SceneFactory sceneFactory);

		void Pop() noexcept;
		std::shared_ptr<Scene> GetCurrentScene();
	private:
		std::shared_ptr<Scene> GetNextScene();
		void CreateScene() noexcept;
		void DestroyScene() noexcept;

		friend class Application;
	};
}