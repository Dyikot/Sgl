#include "Application.h"

namespace Sgl
{
	Application::Application() noexcept
	{
		_current = this;

		if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO) < 0)
		{
			std::cout << SDL_GetError() << '\n';
		}

		if(TTF_Init() < 0)
		{
			std::cout << SDL_GetError() << '\n';
		}

		if(!IMG_Init(IMG_InitFlags::IMG_INIT_PNG | IMG_InitFlags::IMG_INIT_JPG))
		{
			std::cout << SDL_GetError() << '\n';
		}

		if(Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048) < 0)
		{
			std::cout << SDL_GetError() << '\n';
		}
	}

	Application::~Application() noexcept
	{
		delete _audioManager;
		TTF_Quit();
		IMG_Quit();
		Mix_Quit();
		SDL_Quit();
	}

	void Application::Run(Window& window)
	{
		SDL_Event e;
		_window = &window;

		OnStartup(EventArgs());

		while(!window.Scenes.Empty())
		{
			while(SDL_PollEvent(&e))
			{
				switch(e.type)
				{
					case SDL_QUIT: Shutdown(); break;
					default: window.Scenes.Current()->HandleEvent(e);
				}
			}

			window.Render();
			SDL_Delay(_delay);
		}

		OnQuit(EventArgs());
	}

	void Application::Run()
	{
		Window mainWindow;
		Run(mainWindow);
	}

	void Application::Shutdown() noexcept
	{
		if(_window != nullptr)
		{
			_window->Close();
		}
	}

	void Application::OnStartup(const EventArgs& e)
	{
		if(Startup)
		{
			Startup(this, e);
		}
	}

	void Application::OnQuit(const EventArgs& e)
	{
		if(Quit)
		{
			Quit(this, e);
		}		
	}
}