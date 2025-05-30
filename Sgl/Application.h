#pragma once

#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_image.h"
#include "Scene/Scene.h"
#include "Window.h"
#include "Tools/Time/Stopwatch.h"
#include "Data/ResourcesMap.h"
#include "Scene/SceneManager.h"

namespace Sgl
{
	class Application;

	using ApplicationEventHandler = EventHandler<Application, EventArgs>;

	class Application
	{
	public:
		Window Window;
		SceneManager SceneManager;
		ResourcesMap Resources;
		Event<ApplicationEventHandler> Exit;
	private:
		static inline Application* _current;
		bool _running = false;
		std::optional<size_t> _maxFrameRate;
		std::optional<TimeSpan> _maxFrameTime;
	public:
		Application() noexcept;
		~Application() noexcept;

		void SetMaxFrameRate(size_t value) noexcept;
		auto GetMaxFrameRate() const noexcept { return _maxFrameRate; }

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