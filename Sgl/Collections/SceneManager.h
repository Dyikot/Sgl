#pragma once

#include <stack>

namespace Sgl
{
	class Scene;

	class SceneManager: protected std::stack<Scene*>
	{
	public:
		void Load(Scene* scene);
		void UnloadActive();
		bool IsEmpty() const { return empty(); }
		Scene* Active() const { return top(); }
		void UnloadAll();
	};
}