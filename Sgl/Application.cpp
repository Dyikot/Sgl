#include "Application.h"
#include "Base/Log.h"
#include "Base/Time/Timer.h"
#include "Base/Time/Delay.h"

namespace Sgl
{
	Application::Application() noexcept:
		_stylingParent(nullptr)
	{
		_current = this;

		Log::PrintSDLErrorIf(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO) < 0);
		Log::PrintSDLErrorIf(TTF_Init() < 0);
		Log::PrintSDLErrorIf(!IMG_Init(IMG_InitFlags::IMG_INIT_PNG | IMG_InitFlags::IMG_INIT_JPG));
		Log::PrintSDLErrorIf(Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048) < 0);
	}

	Application::~Application() noexcept
	{
		TTF_Quit();
		IMG_Quit();
		Mix_Quit();
		SDL_Quit();
	}

	void Application::SetMaxFPS(size_t value) noexcept
	{
		_maxFPS = value;
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

	void Application::Shutdown()
	{
		_running = false;
		OnShutdown();
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

						case SDL_WINDOWEVENT_SHOWN:
						{
							WindowVisibilityEventArgs args = { .IsVisible = true };
							Window.OnVisibilityChanged(args);
							break;
						}

						case SDL_WINDOWEVENT_HIDDEN:
						{
							WindowVisibilityEventArgs args = { .IsVisible = false };
							Window.OnVisibilityChanged(args);
							break;
						}

						case SDL_WINDOWEVENT_MOVED:
						{
							WindowPositionChangedEventArgs args =
							{
								.Position = 
								{
									.x = e.window.data1,
									.y = e.window.data2
								}
							};
							Window.OnPositionChanged(args);
							break;
						}

						case SDL_WINDOWEVENT_SIZE_CHANGED:
						{
							WindowSizeChangedEventArgs args =
							{
								.Width = static_cast<size_t>(e.window.data1),
								.Height = static_cast<size_t>(e.window.data2)
							};
							Window.OnWindowSizeChanged(args);
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
					break;
				}

				case SDL_TEXTEDITING:
				{
					TextEditingEventArgs args
					{
						.Text = e.edit.text,
						.SelectionStart = static_cast<size_t>(e.edit.start),
						.SelectionLength = static_cast<size_t>(e.edit.length)
					};
					scene->OnTextEditing(args);
					break;
				}

				case SDL_TEXTINPUT:
				{
					TextInputEventArgs args
					{
						.Text = e.text.text
					};
					scene->OnTextInput(args);
					break;
				}

				case SDL_TEXTEDITING_EXT:
				{
					TextEditingEventArgs args
					{
						.Text = e.editExt.text,
						.SelectionStart = static_cast<size_t>(e.edit.start),
						.SelectionLength = static_cast<size_t>(e.edit.length)
					};
					SDL_free(e.editExt.text);
					scene->OnTextEditing(args);
					break;
				}

				case SDL_MOUSEMOTION:
				{
					MouseEventArgs args =
					{
						.Position =
						{
							.x = static_cast<float>(e.button.x),
							.y = static_cast<float>(e.button.y)
						}
					};
					scene->OnMouseMove(args);
					break;
				}

				case SDL_MOUSEBUTTONDOWN:
				{
					MouseButtonEventArgs args =
					{
						.Button = static_cast<MouseButton>(e.button.button),
						.State = static_cast<ButtonState>(e.button.state),
						.ClicksNumber = e.button.clicks
					};
					scene->OnMouseDown(args);
					break;
				}

				case SDL_MOUSEBUTTONUP:
				{
					MouseButtonEventArgs args =
					{
						.Button = static_cast<MouseButton>(e.button.button),
						.State = static_cast<ButtonState>(e.button.state),
						.ClicksNumber = e.button.clicks
					};
					scene->OnMouseUp(args);
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
						.Direction = static_cast<MouseWheelDirection>(e.wheel.direction)
					};
					scene->OnMouseWheelChanged(args);
					break;
				}

				default:
					break;
			}
		}
	}

	void Application::Start()
	{
		OnRun();

		Renderer renderer;
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
			
			_fpsCounter.OnFrameStart();
			delayStopwatch.Restart();

			HandleEvents(scene);
			scene->Process(sceneStopwatch.Elapsed());
			sceneStopwatch.Reset();

			if(Window.IsVisible() || Window.IsRenderableWhenMinimized)
			{
				scene->OnRender(renderer.CreateContext());
				renderer.UpdateScreen();
			}

			if(_maxFPS)
			{
				SleepFor(_maxFrameTime.value() - delayStopwatch.Elapsed());
			}
		}
	}

	void Application::OnRun()
	{
		Started.TryInvoke(*this, EmptyEventArgs);
	}

	void Application::OnShutdown()
	{
		Stopped.TryInvoke(*this, EmptyEventArgs);
	}
}