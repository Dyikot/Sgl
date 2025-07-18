#pragma once

#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_image.h>
#include "Window.h"
#include "Base/FPSCounter.h"
#include "Scene/SceneManager.h"

namespace Sgl
{
	class Application: public IStyleProvider
	{
	private:
		using ApplicationEventHandler = EventHandler<Application>;
	public:
		struct Context
		{
			Application* Current()
			{
				return Application::_current;
			}

			Application* operator->()
			{
				return Application::_current;
			}

			const Application* operator->() const
			{
				return Application::_current;
			}
		};

		Event<ApplicationEventHandler> Started;
		Event<ApplicationEventHandler> Stopped;

		Window Window;
		SceneManager SceneManager;
		ResourcesMap Resources;
		StyleMap Styles;
	protected:
		IStyleProvider* _stylingParent;
	private:
		static inline Application* _current;

		bool _running = false;
		FPSCounter _fpsCounter;
		std::optional<size_t> _maxFPS;
		std::optional<TimeSpan> _maxFrameTime;
	public:
		Application() noexcept;
		Application(const Application&) = delete;
		Application(Application&&) = delete;
		~Application() noexcept;

		void SetMaxFPS(size_t value) noexcept;
		auto GetMaxFPS() const noexcept { return _maxFPS; }

		size_t GetFPS() const { return _fpsCounter.GetFps(); }
		StyleMap& GetStyles() override { return Styles; }
		IStyleProvider* GetStylingParent() { return _stylingParent; }

		void Run();
		void Shutdown();
	protected:
		virtual void OnRun();
		virtual void OnShutdown();
	private:
		void HandleEvents(std::shared_ptr<Scene> scene);
		void Start();
	};	

	inline Application::Context App;
} 