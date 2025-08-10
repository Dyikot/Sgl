#pragma once

#include <stack>
#include <queue>
#include "Scene.h"
#include "../Base/SmartPointers.h"

namespace Sgl
{
	class SceneManager
	{
	private:
		using SceneFactory = Func<Unique<Scene>>;

		std::stack<Unique<Scene>> _scenes;
		std::queue<SceneFactory> _sceneFactoriesQueue;
		size_t _scenesToDestory = 0;
	public:
		SceneManager() = default;
		SceneManager(const SceneManager&) = delete;
		SceneManager(SceneManager&&) = delete;
		~SceneManager();

		template<std::derived_from<Scene> TScene>
		void Push() { _sceneFactoriesQueue.push([] { return NewUnique<TScene>(); }); }
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