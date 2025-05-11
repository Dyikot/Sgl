#pragma once

#include <stack>
#include <queue>
#include "../Graphic/Color.h"
#include "../Graphic/Texture.h"
#include "../ECS/IProcessed.h"
#include "UIElement.h"
#include "ComponentsCollection.h"

namespace Sgl
{	
	class Scene: public UIElement, public ECS::IProcessed
	{
	public:
		ComponentsCollection Components;
	public:
		Scene();
		virtual ~Scene() = default;

		void OnRender(RenderContext renderContext) const override;
	protected:
		void OnMouseMove(const MouseButtonEventArgs& e) override;
		void OnMouseDown(const MouseButtonEventArgs& e) override;
		void OnMouseUp(const MouseButtonEventArgs& e) override;
		virtual void OnTextChanged(const TextChangedEventArgs& e) {}
		virtual void OnTextInput(const TextInputEventArgs& e) {}
	private:
		friend class Application;
	};

	class SceneManager
	{
	private:
		std::stack<std::shared_ptr<Scene>> _scenes;
		std::queue<Func<std::shared_ptr<Scene>>> _scenesBuildersQueue;
		size_t _popScenes = 0;
	public:
		template<std::derived_from<Scene> TScene>
		void Push() noexcept
		{
			_scenesBuildersQueue.push(&std::make_shared<TScene>);
		}

		void Pop() noexcept;
		std::shared_ptr<Scene> GetCurrentScene();
	private:
		void BuildScene() noexcept;
		void DestroyScene() noexcept;
	};
}