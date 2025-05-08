#pragma once

#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_image.h"
#include "Window.h"
#include "UI/Scene.h"
#include "Graphic/Cursor.h"
#include "Data/CallableView.h"
#include "Tools/Time/Stopwatch.h"

namespace Sgl
{
	class Application
	{
	public:
		Event<WindowEventHandler> onWindowInitialized;
		SceneManager sceneManager;
	private:
		bool _running = false;
		std::unique_ptr<Window> _window;
		std::optional<size_t> _maxFrameRate;
		std::optional<TimeSpan> _maxFrameTime;
	public:
		Application() noexcept;
		~Application() noexcept;

		void SetMaxFrameRate(size_t value) noexcept;
		auto GetMaxFrameRate() const noexcept { return _maxFrameRate; }

		void Run();
		void Shutdown() noexcept;
		bool IsRunning() const { return _running; }
	private:
		void HandleEvents(SceneView scene);
		void Start();
	};
} 