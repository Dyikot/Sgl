#pragma once

#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_image.h"
#include "Window.h"
#include "UI/Scene.h"
#include "Graphic/Cursor.h"
#include "Tools/Time/Stopwatch.h"

namespace Sgl
{
	class Application
	{
	public:
		Window window;
		SceneManager sceneManager;
	private:
		static inline Nullable<Application> _current;
		bool _running = false;
		std::optional<size_t> _maxFrameRate;
		std::optional<TimeSpan> _maxFrameTime;
	public:
		static Nullable<Application> Get() { return _current; }

		Application() noexcept;
		~Application() noexcept;

		void SetMaxFrameRate(size_t value) noexcept;
		auto GetMaxFrameRate() const noexcept { return _maxFrameRate; }

		void Run();
		void Shutdown() noexcept;
		bool IsRunning() const { return _running; }
	private:
		void HandleEvents(std::shared_ptr<Scene> scene);
		void Start();
	};

	using App = Application;
} 