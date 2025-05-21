#include "Application.h"
#include "Tools/Time/Timer.h"
#include "Tools/Log.h"
#include "Tools/Time/Delay.h"

namespace Sgl
{
	Application::Application() noexcept:
		Resources(Window.GetRenderer())
	{
		_current = this;

		PrintSDLErrorIf(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO) < 0);
		PrintSDLErrorIf(TTF_Init() < 0);
		PrintSDLErrorIf(!IMG_Init(IMG_InitFlags::IMG_INIT_PNG | IMG_InitFlags::IMG_INIT_JPG));
		PrintSDLErrorIf(Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048) < 0);
	}

	Application::~Application() noexcept
	{
		Exit.TryRaise(*this, ApplicationEventArgs { .Window = Window, .Resources = Resources });
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
		Window.Show();
		Start();
	}

	void Application::Shutdown() noexcept
	{
		_running = false;
	}

	void Application::HandleEvents(std::shared_ptr<Scene> scene)
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
					switch(e.window.event)
					{
						case SDL_WINDOWEVENT_MAXIMIZED:
						{
							WindowStateEventArgs args = { .State = WindowState::Maximized };
							Window.OnWindowStateChanged(args);
							break;
						}

						case SDL_WINDOWEVENT_MINIMIZED:
						{
							WindowStateEventArgs args = { .State = WindowState::Minimized };
							Window.OnWindowStateChanged(args);
							break;
						}

						case SDL_WINDOWEVENT_RESTORED:
						{
							WindowStateEventArgs args = { .State = WindowState::Normal };
							Window.OnWindowStateChanged(args);
							break;
						}

						default:
							break;
					}

					break;
				}

				case SDL_KEYDOWN:
				{
					KeyEventArgs args =
					{
						.State = static_cast<ButtonState>(e.key.state),
						.Key = e.key.keysym
					};

					scene->OnKeyDown(args);
					scene->UIElements.OnKeyDown(args);
					break;
				}

				case SDL_KEYUP:
				{
					KeyEventArgs args =
					{
						.State = static_cast<ButtonState>(e.key.state),
						.Key = e.key.keysym
					};

					scene->OnKeyUp(args);
					scene->UIElements.OnKeyUp(args);
					break;
				}

				case SDL_TEXTEDITING:
				{
					/*scene->OnTextChanged(
						TextChangedEventArgs
						{
							.Text = e.edit.text,
							.SelectionLength = static_cast<size_t>(e.edit.length),
							.SelectionStart = e.edit.start
						}
					);*/
					break;
				}

				case SDL_TEXTINPUT:
				{
					/*scene->OnTextInput(
						TextInputEventArgs
						{
							.Text = e.text.text
						}
					);*/
					break;
				}

				case SDL_TEXTEDITING_EXT:
				{
					/*scene->OnTextChanged(
						TextChangedEventArgs
						{
							.Text = e.editExt.text,
							.SelectionLength = static_cast<size_t>(e.editExt.length),
							.SelectionStart = e.editExt.start
						}
					);
					SDL_free(e.editExt.text);*/
					break;
				}

				case SDL_MOUSEMOTION:
				{
					MouseButtonEventArgs args =
					{
						.Position =
						{
							.x = static_cast<float>(e.button.x),
							.y = static_cast<float>(e.button.y)
						}
					};

					scene->UIElements.OnMouseMove(args);
					break;
				}

				case SDL_MOUSEBUTTONDOWN:
				{
					MouseButtonEventArgs args =
					{
						.Button = static_cast<MouseButton>(e.button.button),
						.State = static_cast<ButtonState>(e.button.state),
						.ClicksNumber = e.button.clicks,
						.Position =
						{
							.x = static_cast<float>(e.button.x),
							.y = static_cast<float>(e.button.y)
						}
					};

					scene->UIElements.OnMouseDown(args);
					break;
				}

				case SDL_MOUSEBUTTONUP:
				{
					MouseButtonEventArgs args =
					{
						.Button = static_cast<MouseButton>(e.button.button),
						.State = static_cast<ButtonState>(e.button.state),
						.ClicksNumber = e.button.clicks,
						.Position =
						{
							.x = static_cast<float>(e.button.x),
							.y = static_cast<float>(e.button.y)
						}
					};

					scene->UIElements.OnMouseUp(args);
					break;
				}

				case SDL_MOUSEWHEEL:
				{
					MouseWheelEventArgs args =
					{
						.Position =
						{
							.x = static_cast<float>(e.button.x),
							.y = static_cast<float>(e.button.y)
						},
						.ScrolledHorizontally = e.wheel.preciseX,
						.ScrolledVertically = e.wheel.preciseY,
						.Direction = SDL_MouseWheelDirection(e.wheel.direction)
					};

					scene->UIElements.OnMouseWheel(args);
					break;
				}

				default:
					break;
			}
		}
	}

	void Application::Start()
	{
		auto& renderer = Window.GetRenderer();
		renderer.SetBlendMode(SDL_BLENDMODE_BLEND);

		Stopwatch delayStopwatch, sceneStopwatch;
		sceneStopwatch.Start();

		while(_running)
		{
			auto scene = SceneManager.GetNextScene();

			if(scene == nullptr)
			{
				Shutdown();
				continue;
			}
			
			delayStopwatch.Restart();
			HandleEvents(scene);
			scene->OnProcessing(sceneStopwatch.Elapsed());
			sceneStopwatch.Reset();

			if(Window.IsVisible() || Window.IsRenderableWhenMinimized)
			{
				scene->OnRender(RenderContext(renderer));
				renderer.UpdateScreen();
			}

			if(_maxFrameRate)
			{
				SleepFor(_maxFrameTime.value() - delayStopwatch.Elapsed());
			}
		}
	}
}