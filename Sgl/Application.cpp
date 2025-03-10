#include "Application.h"
#include "Window.h"
#include "Tools/Log.h"

namespace Sgl
{
	Application::Application() noexcept
	{
		_current = this;

		if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO) < 0)
		{
			Log::PrintSDLError();
		}

		if(TTF_Init() < 0)
		{
			Log::PrintSDLError();
		}

		if(!IMG_Init(IMG_InitFlags::IMG_INIT_PNG | IMG_InitFlags::IMG_INIT_JPG))
		{
			Log::PrintSDLError();
		}

		if(Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048) < 0)
		{
			Log::PrintSDLError();
		}
	}

	Application::~Application() noexcept
	{
		_current = nullptr;
		TTF_Quit();
		IMG_Quit();
		Mix_Quit();
		SDL_Quit();
	}

	void Application::SetMaxFrameRate(size_t value)
	{
		_maxFrameRate = value > MaxFrameRate ? MaxFrameRate : value;
		_maxFrameTime = TimeSpan::FromMilliseconds(1000.f / _maxFrameRate.value());
	}

	Window* Application::GetWindow() const
	{
		return _window;
	}

	void Application::Run()
	{
		if(_running)
		{
			return;
		}
		
		Window window(*this);
		_window = &window;
		WindowConfigurator(window);
		window.Show();

		OnStartup(EventArgs());
		Start();
		OnQuit(EventArgs());
	}

	void Application::Shutdown() noexcept
	{
		_running = false;
	}

	void Application::OnStartup(const EventArgs& e)
	{
		_running = true;
		Startup.TryInvoke(this, e);
	}

	void Application::OnQuit(const EventArgs& e)
	{
		_window = nullptr;
		_running = false;
		Quit.TryInvoke(this, e);
	}

	void Application::HandleEvents()
	{		
		SDL_Event e;
		while(SDL_PollEvent(&e))
		{
			switch(e.type)
			{
				case SDL_QUIT:
				{
					Shutdown();
					break;
				}

				default:
				{
					_window->SceneManager.HandleSceneEvents(e);
					break;
				}
			}
		}
	}

	void Application::Start()
	{
		while(_running)
		{
			switch(_window->SceneManager.UpdateState())
			{
				case SceneState::Loading:
					continue;
				case SceneState::Loaded:
					break;
				case SceneState::Unloaded:
					return;
			}

			_stopwatch.Restart();
			HandleEvents();
			_window->SceneManager.ProcessScene(_stopwatch.Elapsed());
			if(_window->IsVisible() || _window->AllowRenderMinimizedWindow)
			{
				_window->SceneManager.RenderScene();
			}

			if(_maxFrameRate)
			{
				auto delayMs = (_maxFrameTime.value() - _stopwatch.Elapsed()).Milliseconds();	
				SDL_Delay(delayMs > 0 ? delayMs : 0);
			}
		}
	}
}