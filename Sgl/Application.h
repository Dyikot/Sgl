#pragma once

#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_image.h"
#include "Scene/SceneManager.h"
#include "Window.h"
#include "Tools/FPSCounter.h"
#include "Data/Ranges/ResourcesMap.h"

namespace Sgl
{
	class Application
	{
	public:
		using ApplicationEventHandler = EventHandler<Application, EventArgs>;

		Window Window;
		SceneManager SceneManager;
		ResourcesMap Resources;

		Event<ApplicationEventHandler> Exit;
	private:
		static inline Application* _current;

		bool _running = false;
		FPSCounter _fpsCounter;
		std::optional<size_t> _maxFPS;
		std::optional<TimeSpan> _maxFrameTime;
	public:
		Application() noexcept;
		~Application() noexcept;

		void SetMaxFPS(size_t value) noexcept;
		auto GetMaxFPS() const noexcept { return _maxFPS; }

		size_t GetFPS() const { return _fpsCounter.GetFps(); }

		void Run();
		void Shutdown() noexcept;
	private:
		void HandleEvents(std::shared_ptr<Scene> scene);
		void Start();

		friend class ApplicationContext;
	};

	struct ApplicationContext
	{
		Application* operator->()
		{
			return Application::_current;
		}

		const Application* operator->() const
		{
			return Application::_current;
		}
	};

	inline ApplicationContext App;
} 