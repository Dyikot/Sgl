#pragma once

#include <stack>
#include <queue>
#include "../Appearance/Color.h"
#include "../Appearance/Texture.h"
#include "../ECS/IProcessed.h"
#include "Panels/Panel.h"

namespace Sgl
{	
	class Window;

	class Scene: public UIElement, public IProcessed
	{
	public:				
		ComponentSet<Panel> Panels;
		Sgl::Window& Window;
	private:
		Panel* _mouseOverPanel = nullptr;
		bool _unload = false;
	public:
		explicit Scene(Sgl::Window& window);
		virtual ~Scene() = default;

		Event<UIEventHandler> Loaded;
		Event<UIEventHandler> Unloaded;

		void OnRender(RenderContext& renderContext) override;
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

		template<typename TScene, typename... TArgs> requires std::derived_from<TScene, Scene>
		void Load(TArgs&&... args)
		{
			_scenesQueue.push(std::make_shared<TScene>(Window, std::forward<TArgs>(args)...));
		}

		void Unload();
		SceneState UpdateState();
		void RenderScene();
		void ProcessScene(TimeSpan elapsed);
		void HandleSceneEvents(SDL_Event& e);
	protected:
		void LoadScene();
		void UnloadScene();
	};
}