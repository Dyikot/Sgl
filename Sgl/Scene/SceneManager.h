#pragma once

#include <stack>
#include <queue>
#include "Scene.h"

namespace Sgl
{
	class SceneManager
	{
	private:
		using SceneFactory = Func<std::unique_ptr<Scene>>;

		std::stack<std::unique_ptr<Scene>> _scenes;
		std::queue<SceneFactory> _sceneFactoriesQueue;
		size_t _scenesToDestroy = 0;
	public:
		SceneManager() = default;
		SceneManager(const SceneManager&) = delete;
		SceneManager(SceneManager&&) = delete;
		~SceneManager();

		template<std::derived_from<Scene> TScene>
		void Push() { _sceneFactoriesQueue.push([] { return std::make_unique<TScene>(); }); }
		void Push(SceneFactory sceneFactory);

		void Pop() noexcept;
		void Clear();
		Scene* GetCurrentScene();
	private:
		Scene* GetNextScene();
		void CreateScene();
		void DestroyScene();

		friend class Application;
	};
}