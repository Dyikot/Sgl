#pragma once

#include <stack>
#include "../UI/Scene.h"

namespace Sgl
{
	class SceneCollection: protected std::stack<Scene*>
	{
	public:
		void Push(Scene* scene);
		void Pop();
		bool Empty() const;
		Scene* const Current() const;
		void Clear();
	};
}