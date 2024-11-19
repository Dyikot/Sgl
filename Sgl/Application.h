#pragma once

#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_image.h"
#include "Audio/AudioManager.h"
#include "Window.h"

namespace Sgl
{
	class Application
	{
	public:
		using ApplicationEventHandler = std::function<void(Application*, const EventArgs&)>;
		AudioManager AudioManager;
		Window* MainWindow = nullptr;
	protected:
		size_t _startTimePoint;
	private:
		inline static Application* _current = nullptr;
		static constexpr uint32_t MaxFrameRate = 360;
		std::optional<uint32_t> _maxFrameRate = std::nullopt;
	public:
		static Application* const Current() { return _current; }

		Application() noexcept;
		~Application() noexcept;

		void SetMaxFrameRate(uint32_t value);
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
			return 1000.f * static_cast<float>(SDL_GetPerformanceCounter() - _startTimePoint)
						  / SDL_GetPerformanceFrequency();
		}
		float MaxFrameTimeMs() const { return 1000.f / _maxFrameRate.value(); }
	};
} 