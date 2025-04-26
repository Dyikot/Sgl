#include "Application.h"
#include "Tools/Time/Timer.h"
#include "Tools/Log.h"
#include "Tools/Time/Delay.h"

namespace Sgl
{
	Application::Application() noexcept
	{
		PrintSDLErrorIf(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO) < 0);
		PrintSDLErrorIf(TTF_Init() < 0);
		PrintSDLErrorIf(!IMG_Init(IMG_InitFlags::IMG_INIT_PNG | IMG_InitFlags::IMG_INIT_JPG));
		PrintSDLErrorIf(Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048) < 0);
	}

	Application::~Application() noexcept
	{
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
		if(_running)
		{
			return;
		}
		
		_running = true;
		_window = std::make_unique<Window>();
		onWindowInitialized.TryRaise(*_window, EventArgs());
		_window->Show();

		Start();
	}

	void Application::Shutdown() noexcept
	{
		_running = false;
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
					sceneManager.HandleSceneEvents(e);
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
			switch(sceneManager.UpdateState())
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
			sceneManager.ProcessScene(sceneStopwatch.Elapsed());
			sceneStopwatch.Reset();

			if(_window->IsVisible() || _window->canRenderInMinimizedMode)
			{
				sceneManager.RenderScene(renderContext);
			}

			if(_maxFrameRate)
			{
				SleepFor(_maxFrameTime.value() - delayStopwatch.Elapsed());
			}
		}
	}
}