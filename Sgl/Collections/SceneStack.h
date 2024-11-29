#pragma once

#include <stack>

namespace Sgl
{
	class Scene;

	class SceneStack: protected std::stack<Scene*>
	{
	public:
		void Push(Scene* scene);
		void Pop();
		bool Empty() const;
		Scene* const Current() const;
		void Clear();
	};
}