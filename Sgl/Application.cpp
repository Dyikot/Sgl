#include "Application.h"
#include "Window.h"
#include "Tools/Time/Timer.h"
#include "Tools/Log.h"
#include "Tools/Time/Delay.h"

namespace Sgl
{
	Application::Application() noexcept
	{
		_current = this;

		PrintSDLErrorIf(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO) < 0);
		PrintSDLErrorIf(TTF_Init() < 0);
		PrintSDLErrorIf(!IMG_Init(IMG_InitFlags::IMG_INIT_PNG | IMG_InitFlags::IMG_INIT_JPG));
		PrintSDLErrorIf(Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048) < 0);
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
		_maxFrameRate = value;
		_maxFrameTime = TimeSpan(1e9 / _maxFrameRate.value());
	}

	void Application::Run()
	{
		Run([](Window&) {});
	}

	void Application::Run(ActionView<Window&> windowConfigurer)
	{
		if(_running)
		{
			return;
		}

		Window window(*this);
		_window = &window;
		windowConfigurer(window);
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
		Startup.TryRaise(*this, e);
	}

	void Application::OnQuit(const EventArgs& e)
	{
		_window = nullptr;
		_running = false;
		Quit.TryRaise(*this, e);
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
		Stopwatch delayStopwatch, sceneStopwatch;
		auto renderContext = _window->GetRenderContext();

		renderContext.SetBlendMode(SDL_BLENDMODE_BLEND);
		sceneStopwatch.Start();

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
			
			delayStopwatch.Restart();
			HandleEvents();
			_window->SceneManager.ProcessScene(sceneStopwatch.Elapsed());
			sceneStopwatch.Reset();

			if(_window->IsVisible() || _window->CanRenderInMinimizedMode)
			{
				_window->SceneManager.RenderScene(renderContext);
			}

			if(_maxFrameRate)
			{
				SleepFor(_maxFrameTime.value() - delayStopwatch.Elapsed());
			}
		}
	}
}