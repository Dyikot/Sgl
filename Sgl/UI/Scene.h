#pragma once

#include <stack>
#include <queue>
#include "../ECS/IProcessed.h"
#include "UIElement/UIElement.h"

namespace Sgl
{	
	class Scene: public VisualElement, public ECS::IProcessed, public IKeyEventsListener
	{
	public:
		Style ClassStyle;
		UIElementsCollection UIElements;
		Event<KeyEventHandler> KeyUp;
		Event<KeyEventHandler> KeyDown;
	public:
		Scene();
		virtual ~Scene() = default;

		void OnRender(RenderContext renderContext) const final;
	protected:
		void OnKeyDown(const KeyEventArgs& e) override
		{
			KeyDown.TryRaise(*this, e);
		}

		void OnKeyUp(const KeyEventArgs& e) override
		{
			KeyUp.TryRaise(*this, e);
		}
	private:
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