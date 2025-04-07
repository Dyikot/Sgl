#pragma once

#include <stack>
#include <queue>
#include "../Graphic/Color.h"
#include "../Graphic/Texture.h"
#include "../ECS/IProcessed.h"
#include "Component.h"
#include "../Data/CallableView.h"

namespace Sgl
{	
	class Window;
	class Scene;

	template<typename TScene>
	concept CScene = std::derived_from<TScene, Scene>;

	using SceneEventHandler = EventHandler<Scene, EventArgs>;

	class Scene: public UIElement, public ECS::IProcessed
	{
	public:				
		Sgl::Window& Window;
		ComponentsCollection Components;
	public:
		explicit Scene(Sgl::Window& window);
		virtual ~Scene() = default;

		Event<SceneEventHandler> Loaded;
		Event<SceneEventHandler> Unloaded;

		void OnRender(RenderContext& renderContext) const override;
		void SwitchCursorOn(const Cursor& cursor) override;
		void SwitchCursorOnDefault() override;
		UIElement& GetRootElement() override { return *this; }
	protected:
		void OnMouseMove(const MouseButtonEventArgs& e) override;
		void OnMouseDown(const MouseButtonEventArgs& e) override;
		void OnMouseUp(const MouseButtonEventArgs& e) override;
		virtual void OnLoaded(const EventArgs& e);
		virtual void OnUnloaded(const EventArgs& e);
		virtual void OnTextChanged(const TextChangedEventArgs& e) {};
		virtual void OnTextInput(const TextInputEventArgs& e) {};
	private:
		friend class SceneManager;
		friend class Application;
	};

	enum SceneState
	{
		Loading, Loaded, Unloaded
	};

	class SceneManager
	{
	public:
		Sgl::Window& Window;
	protected:
		std::stack<std::shared_ptr<Scene>> _scenes;
		std::queue<std::shared_ptr<Scene>> _scenesQueue;
		size_t _scenesToUnload = 0;
	public:
		SceneManager(Sgl::Window& window);
		~SceneManager();

		template<CScene TScene>
		void Load()
		{
			_scenesQueue.push(std::make_shared<TScene>(Window));
		}

		template<CScene TScene>
		void Load(ActionView<Scene&> sceneFactory)
		{
			_scenesQueue.push(std::make_shared<TScene>(Window));
			sceneFactory(*_scenesQueue.back());
		}

		void Unload();
		SceneState UpdateState();
		void RenderScene(RenderContext& renderContext);
		void ProcessScene(TimeSpan elapsed);
		void HandleSceneEvents(SDL_Event& e);
	protected:
		void LoadScene();
		void UnloadScene();
	};
}