#pragma once

#include <stack>
#include "../UI/Scene.h"

namespace Sgl
{
	class Scene;

	class SceneStack: protected std::stack<std::shared_ptr<Scene>>
	{
	public:
		~SceneStack();

		void Push(Scene* scene);
		void Pop();
		bool Any() const { return !empty(); }
		bool IsCurrentClosed();
		Scene* Current() const { return top().get(); }
		void Clear();
		size_t Count() const;
	};
}