#include "Application.h"
#include "Render/Shapes.h"
#include "Window.h"
#include "Tools/Log.h"

namespace Sgl
{
	Application::Application() noexcept
	{
		_current = this;

		if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO) < 0)
		{
			PrintSDLError();
		}

		if(TTF_Init() < 0)
		{
			PrintSDLError();
		}

		if(!IMG_Init(IMG_InitFlags::IMG_INIT_PNG | IMG_InitFlags::IMG_INIT_JPG))
		{
			PrintSDLError();
		}

		if(Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048) < 0)
		{
			PrintSDLError();
		}

		Cursors::Initialize();
	}

	Application::~Application() noexcept
	{
		_current = nullptr;
		TTF_Quit();
		IMG_Quit();
		Mix_Quit();
		SDL_Quit();
	}

	void Application::SetDefaultCursor()
	{
		SetCursor(_defaultCursor);
	}

	void Application::SetCursor(const Cursor& cursor)
	{
		if(_activeCursor.get() != cursor)
		{
			SDL_SetCursor(cursor);
		}

		_activeCursor = cursor;
	}

	void Application::SetMaxFrameRate(size_t value)
	{
		_maxFrameRate = value > MaxFrameRate? MaxFrameRate : value;
		_maxFrameTime = TimeSpan(1000.f / _maxFrameRate.value());
	}

	Window* Application::GetWindow() const
	{
		return _window;
	}

	const Cursor& Application::GetCursor() const
	{
		return _activeCursor;
	}

	void Application::Run()
	{
		if(_running)
		{
			return;
		}

		Window window(*this);
		_window = &window;
		WindowInitializer(window);

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

		if(Startup)
		{
			Startup(this, e);
		}
	}

	void Application::OnQuit(const EventArgs& e)
	{
		_window = nullptr;
		_running = false;

		if(Quit)
		{
			Quit(this, e);
		}		
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

				case SDL_WINDOWEVENT:
				{
					_window->OnStateChanged(EventArgs());
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
			if(_window->IsVisible())
			{
				_window->SceneManager.ProcessScene(_stopwatch.Elapsed());
				_window->SceneManager.RenderScene();
			}

			if(_maxFrameRate)
			{
				auto delay = _maxFrameTime.value() - _stopwatch.Elapsed();	
				if(delay.Milliseconds() > 0)
				{
					SDL_Delay(delay.Milliseconds());
				}
			}
		}
	}
}