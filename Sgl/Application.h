#pragma once

#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_image.h"
#include "Audio/AudioManager.h"
#include "Tools/Time/Stopwatch.h"
#include "Events/Event.h"
#include "Graphic/Cursor.h"
#include "UI/Scene.h"

namespace Sgl
{
	class Application;
	class Window;

	using ApplicationEventHandler = EventHandler<Application, EventArgs>;

	class Application
	{
	public:
		AudioManager AudioManager;
		std::function<void(Window&)> WindowConfigurator;
	protected:
		static constexpr size_t MaxFrameRate = 360;

		std::optional<size_t> _maxFrameRate;
		std::optional<TimeSpan> _maxFrameTime;
	private:
		inline static Application* _current = nullptr;
		Window* _window = nullptr;
		bool _running = false;
	public:
		Application() noexcept;
		~Application() noexcept;

		static Application* Current() { return _current; }

		void SetMaxFrameRate(size_t value);

		Window* GetWindow() const;
		std::optional<size_t> GetMaxFrameRate() const { return _maxFrameRate; }

		Event<ApplicationEventHandler> Startup;
		Event<ApplicationEventHandler> Quit;

		void Run();
		void Shutdown() noexcept;
		bool IsRunning() const { return _running; }
		void SleepFor(TimeSpan timespan);
	protected:
		virtual void OnStartup(const EventArgs& e);
		virtual void OnQuit(const EventArgs& e);

		void HandleEvents();
	private:
		void Start();
	};
} 