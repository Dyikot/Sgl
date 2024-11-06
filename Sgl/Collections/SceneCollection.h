#pragma once

#include <stack>
#include "../UI/Scene.h"

namespace Sgl
{
	class SceneCollection: protected std::stack<Scene*>
	{
	public:
		void Add(Scene* scene);
		void RemoveCurrent();
		bool Empty() const;
		Scene* const Current() const;
		void Clear();
	};
}