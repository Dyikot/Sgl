#pragma once

#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_image.h"
#include "Audio/AudioManager.h"
#include "Tools/Time/Stopwatch.h"
#include "Events/Event.h"
#include "Appearance/Cursor.h"
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
		std::function<void(Window&)> WindowInitializer;
	protected:
		static constexpr size_t MaxFrameRate = 360;

		Stopwatch _stopwatch;
		std::optional<size_t> _maxFrameRate = std::nullopt;
		std::optional<TimeSpan> _maxFrameTime = std::nullopt;
		std::reference_wrapper<const Cursor> _defaultCursor = Cursors::Arrow();
		std::reference_wrapper<const Cursor> _activeCursor = _defaultCursor;
	private:
		inline static Application* _current = nullptr;
		Window* _window = nullptr;
		bool _running = false;
	public:
		Application() noexcept;
		~Application() noexcept;

		static Application* Current() { return _current; }

		void SetDefaultCursor();
		void SetCursor(const Cursor& cursor);
		void SetMaxFrameRate(size_t value);

		Window* GetWindow() const;
		const Cursor& GetCursor() const;
		std::optional<size_t> GetMaxFrameRate() const { return _maxFrameRate; }

		Event<ApplicationEventHandler> Startup;
		Event<ApplicationEventHandler> Quit;

		void Run();
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