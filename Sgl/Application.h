#pragma once

#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_image.h"
#include "Tools/Log.h"
#include "Audio/AudioManager.h"
#include "Window.h"

namespace Sgl
{
	class Application
	{
	public:
		using ApplicationEventHandler = EventHandler<Application, EventArgs>;
		AudioManager AudioManager;
		Window* MainWindow = nullptr;
	protected:
		static constexpr uint32_t MaxFrameRate = 360;
		std::optional<uint32_t> _maxFrameRate = std::nullopt;
		size_t _start = 0;
		const Cursor* _appicationCursor = &Cursors::Arrow;
		const Cursor* _activeCursor = _appicationCursor;
	private:
		inline static Application* _current = nullptr;
	public:
		static Application* const Current() { return _current; }

		Application() noexcept;
		~Application() noexcept;

		void SetApplicationCursor();
		void SetCursor(const Cursor& cursor);
		void SetMaxFrameRate(uint32_t value);

		const Cursor& GetCursor() const;
		std::optional<uint32_t> GetMaxFrameRate() const { return _maxFrameRate; }

		Event<ApplicationEventHandler> Startup;
		Event<ApplicationEventHandler> Quit;

		void Run(Window& window);
		void Run();
		void Shutdown() noexcept;
	protected:
		virtual void OnStartup(const EventArgs& e);
		virtual void OnQuit(const EventArgs& e);

		float ElapsedMs() const 
		{
			return 1000.f * static_cast<float>(SDL_GetPerformanceCounter() - _start)
						  / SDL_GetPerformanceFrequency();
		}
		float MaxFrameTimeMs() const { return 1000.f / _maxFrameRate.value(); }
	};
} 