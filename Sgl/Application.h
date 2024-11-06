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
	protected:
		Window* _window = nullptr;
		AudioManager* _audioManager = nullptr;
		uint32_t _delay = 10;
	private:
		inline static Application* _current = nullptr;
	public:
		static Application* const Current() { return _current; }

		Application() noexcept;
		~Application() noexcept;

		Window* const GetWindow() 
		{ 
			return _window;
		}
		template<typename T = AudioManager>
		T* const GetAudioManager() const 
		{ 
			return static_cast<T* const>(_audioManager); 
		}

		Event<ApplicationEventHandler> Startup;
		Event<ApplicationEventHandler> Quit;

		void Run(Window* window);
		void Run();
		void Shutdown() noexcept;
	protected:
		virtual void OnStartup(const EventArgs& e);
		virtual void OnQuit(const EventArgs& e);
	};
}