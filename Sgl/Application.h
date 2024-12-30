#pragma once

#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_image.h"
#include "Tools/Log.h"
#include "Audio/AudioManager.h"
#include "Window.h"
#include "Tools/Time/Stopwatch.h"

namespace Sgl
{
	class Application
	{
	public:
		using ApplicationEventHandler = EventHandler<Application, EventArgs>;

		AudioManager AudioManager;
	protected:
		static constexpr size_t MaxFrameRate = 360;

		Stopwatch _stopwatch;
		std::optional<size_t> _maxFrameRate = std::nullopt;
		std::optional<TimeSpan> _maxFrameTime = std::nullopt;
		std::reference_wrapper<const Cursor> _defaultCursor = Cursors::Arrow;
		std::reference_wrapper<const Cursor> _activeCursor = _defaultCursor;
	private:
		inline static Application* _current = nullptr;
		Window* _window = nullptr;
		bool _isRunning = false;
	public:
		static Application* Current() { return _current; }

		Application() noexcept;
		~Application() noexcept;

		void SetDefaultCursor();
		void SetCursor(const Cursor& cursor);
		void SetMaxFrameRate(size_t value);

		Window* GetWindow() const { return _window; }
		const Cursor& GetCursor() const;
		std::optional<size_t> GetMaxFrameRate() const { return _maxFrameRate; }

		Event<ApplicationEventHandler> Startup;
		Event<ApplicationEventHandler> Quit;

		void Run(Window& window);
		void Run();
		void Shutdown() noexcept;
	protected:
		virtual void OnStartup(const EventArgs& e);
		virtual void OnQuit(const EventArgs& e);

		void HandleEvents(Scene* scene);
	};
} 