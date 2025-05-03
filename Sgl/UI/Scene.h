#pragma once

#include <stack>
#include <queue>
#include "../Graphic/Color.h"
#include "../Graphic/Texture.h"
#include "../ECS/IProcessed.h"
#include "../Data/CallableView.h"
#include "UIElement.h"
#include "ComponentsCollection.h"

namespace Sgl
{	
	class Scene;

	template<typename TScene>
	concept CScene = std::derived_from<TScene, Scene>;

	class Scene: public UIElement, public ECS::IProcessed
	{
	public:
		ComponentsCollection components = ComponentsCollection(*this);
	public:
		Scene() = default;

		template<CAction<Style&>... TStyleSelector>
		Scene(TStyleSelector&&... styleSelectors):
			UIElement(std::forward<TStyleSelector>(styleSelectors)...)
		{}

		virtual ~Scene() = default;

		void OnRender(RenderContext renderContext) const override;
	protected:
		void OnMouseMove(const MouseButtonEventArgs& e) override;
		void OnMouseDown(const MouseButtonEventArgs& e) override;
		void OnMouseUp(const MouseButtonEventArgs& e) override;
		virtual void OnTextChanged(const TextChangedEventArgs& e) {}
		virtual void OnTextInput(const TextInputEventArgs& e) {}
	private:
		friend class SceneManager;
	};

	enum SceneState
	{
		Loading, Loaded, Unloaded
	};

	class SceneManager
	{
	private:
		using SceneView = std::shared_ptr<Scene>;

		std::stack<SceneView> _scenes;
		std::queue<Func<SceneView>> _scenesBuildersQueue;
		size_t _scenesToUnload = 0;
	public:
		SceneManager() = default;
		~SceneManager();

		template<CScene TScene>
		void Load()
		{
			_scenesBuildersQueue.push([] { return std::make_shared<TScene>(); });
		}

		void Unload();
	private:
		void LoadScene();
		void UnloadScene();
		void ProcessScene(TimeSpan elapsed);
		void RenderScene(RenderContext renderContext);
		void HandleSceneEvents(SDL_Event& e);
		SceneState UpdateState();

		friend class Application;
	};
}