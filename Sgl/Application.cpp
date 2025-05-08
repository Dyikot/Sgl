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

	void Application::SetMaxFrameRate(size_t value) noexcept
	{
		_maxFrameRate = value;
		_maxFrameTime = TimeSpan(1e9 / value);
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

	void Application::HandleEvents(SceneView scene)
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

				case SDL_KEYDOWN:
				{
					scene->OnKeyDown(
						KeyEventArgs
						{
							.state = static_cast<ButtonState>(e.key.state),
							.key = e.key.keysym
						}
					);

					break;
				}

				case SDL_KEYUP:
				{
					scene->OnKeyUp(
						KeyEventArgs
						{
							.state = static_cast<ButtonState>(e.key.state),
							.key = e.key.keysym
						}
					);

					break;
				}

				case SDL_TEXTEDITING:
				{
					scene->OnTextChanged(
						TextChangedEventArgs
						{
							.text = e.edit.text,
							.selectionLength = static_cast<size_t>(e.edit.length),
							.selectionStart = e.edit.start
						}
					);

					break;
				}

				case SDL_TEXTINPUT:
				{
					scene->OnTextInput(
						TextInputEventArgs
						{
							.text = e.text.text
						}
					);

					break;
				}

				case SDL_TEXTEDITING_EXT:
				{
					scene->OnTextChanged(
						TextChangedEventArgs
						{
							.text = e.editExt.text,
							.selectionLength = static_cast<size_t>(e.editExt.length),
							.selectionStart = e.editExt.start
						}
					);
					SDL_free(e.editExt.text);

					break;
				}

				case SDL_MOUSEMOTION:
				{
					scene->OnMouseMove(
						MouseButtonEventArgs
						{
							.position =
							{
								.x = static_cast<float>(e.button.x),
								.y = static_cast<float>(e.button.y)
							}
						}
					);

					break;
				}

				case SDL_MOUSEBUTTONDOWN:
				{
					scene->OnMouseDown(
						MouseButtonEventArgs
						{
							.button = static_cast<MouseButton>(e.button.button),
							.state = static_cast<ButtonState>(e.button.state),
							.clicksCount = e.button.clicks,
							.position =
							{
								.x = static_cast<float>(e.button.x),
								.y = static_cast<float>(e.button.y)
							}
						}
					);

					break;
				}

				case SDL_MOUSEBUTTONUP:
				{
					scene->OnMouseUp(
						MouseButtonEventArgs
						{
							.button = static_cast<MouseButton>(e.button.button),
							.state = static_cast<ButtonState>(e.button.state),
							.clicksCount = e.button.clicks,
							.position =
							{
								.x = static_cast<float>(e.button.x),
								.y = static_cast<float>(e.button.y)
							}
						}
					);

					break;
				}

				case SDL_MOUSEWHEEL:
				{
					scene->OnMouseWheel(
						MouseWheelEventArgs
						{
							.position =
							{
								.x = static_cast<float>(e.button.x),
								.y = static_cast<float>(e.button.y)
							},
							.scrolledHorizontally = e.wheel.preciseX,
							.scrolledVertically = e.wheel.preciseY,
							.direction = SDL_MouseWheelDirection(e.wheel.direction)
						}
					);

					break;
				}

				default:
					break;
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
			auto scene = sceneManager.GetCurrentScene();

			if(scene == nullptr)
			{
				Shutdown();
				continue;
			}
			
			delayStopwatch.Restart();
			HandleEvents(scene);
			scene->OnProcessing(sceneStopwatch.Elapsed());
			sceneStopwatch.Reset();

			if(_window->IsVisible() || _window->canRenderInMinimizedMode)
			{
				scene->OnRender(renderContext);
				SDL_RenderPresent(renderContext);
			}

			if(_maxFrameRate)
			{
				SleepFor(_maxFrameTime.value() - delayStopwatch.Elapsed());
			}
		}
	}
}