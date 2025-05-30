#pragma once

#include <stack>
#include <queue>
#include "Scene.h"

namespace Sgl
{
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