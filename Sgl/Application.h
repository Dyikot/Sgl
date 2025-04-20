#pragma once

#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_image.h"
#include "Tools/Time/Stopwatch.h"
#include "Events/Event.h"
#include "Graphic/Cursor.h"
#include "UI/Scene.h"
#include "Data/CallableView.h"

namespace Sgl
{
	class Application;
	class Window;

	using ApplicationEventHandler = EventHandler<Application, EventArgs>;

	class Application
	{
	protected:
		std::optional<size_t> _maxFrameRate;
		std::optional<TimeSpan> _maxFrameTime;
	private:
		inline static Nullable<Application> _current;
		Nullable<Window> _window;
		bool _running = false;
	public:
		Application() noexcept;
		~Application() noexcept;

		static auto Current() { return _current; }

		void SetMaxFrameRate(size_t value);

		auto GetWindow() const { return _window; }
		auto GetMaxFrameRate() const { return _maxFrameRate; }

		Event<ApplicationEventHandler> Startup;
		Event<ApplicationEventHandler> Quit;

		void Run();
		void Run(ActionView<Window&> windowConfigurer);
		void Shutdown() noexcept;
		bool IsRunning() const { return _running; }
	protected:
		virtual void OnStartup(const EventArgs& e);
		virtual void OnQuit(const EventArgs& e);

		void HandleEvents();
	private:
		void Start();
	};
} 