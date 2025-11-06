#pragma once

#include <stack>
#include <queue>
#include "Scene.h"
#include "../Base/Ref.h"

namespace Sgl
{
	class SceneManager
	{
	private:
		using SceneFactory = Func<Ref<Scene>>;

		std::stack<Ref<Scene>> _scenes;
		std::queue<SceneFactory> _sceneFactoriesQueue;
		size_t _scenesToDestroy = 0;
	public:
		SceneManager() = default;
		SceneManager(const SceneManager&) = delete;
		SceneManager(SceneManager&&) = delete;
		~SceneManager();

		template<std::derived_from<Scene> TScene>
		void Push() { _sceneFactoriesQueue.push([] { return New<TScene>(); }); }
		void Push(SceneFactory sceneFactory);

		void Pop() noexcept;
		void Clear();
		Ref<Scene> GetCurrentScene();
	private:
		Ref<Scene> GetNextScene();
		void CreateScene();
		void DestroyScene();

		friend class Application;
	};
}