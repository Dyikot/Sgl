#include "Application.h"
#include "Base/Log.h"
#include "Base/Time/Timer.h"
#include "Base/Time/Delay.h"
#include "Base/Localization/CSVParser.h"
#include "Base/Localization/StringLocalizer.h"

namespace Sgl
{
	Application::Application() noexcept
	{
		_current = this;

		Log::PrintSDLErrorIf(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO) < 0);
		Log::PrintSDLErrorIf(TTF_Init() < 0);
		Log::PrintSDLErrorIf(!IMG_Init(IMG_InitFlags::IMG_INIT_PNG | IMG_InitFlags::IMG_INIT_JPG));
		Log::PrintSDLErrorIf(Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048) < 0);
	}

	Application::~Application()
	{
		SceneManager.Clear();
		SDL_DestroyRenderer(Window.GetSDLRenderer());
		SDL_DestroyWindow(Window.GetSDLWindow());
		TTF_Quit();
		IMG_Quit();
		Mix_Quit();
		SDL_Quit();
	}

	void Application::SetFpsLimit(size_t value) noexcept
	{
		_fpsLimit = value;
		_frameTimeLimit = TimeSpan(1e9 / value);
	}

	size_t Application::GetFpsLimit() const noexcept
	{
		return _fpsLimit;
	}

	size_t Application::GetFps() const
	{
		return _fpsCounter.GetFps();
	}

	void Application::SetCulture(std::string value)
	{
		_culture = std::move(value);

		if(_localizer)
		{
			_localizer->SetCulture(_culture);
		}
	}

	void Application::SetLocalizer(std::string csvFile, char delimeter)
	{
		CSVParser csvParser(std::move(csvFile), delimeter);
		std::vector<std::string> headers;
		std::vector<std::string> records;

		if(csvParser.ParseTo(headers, records))
		{
			SetLocalizer(std::make_unique<StringLocalizer>(std::move(headers), std::move(records)));
		}
	}

	void Application::SetLocalizer(std::unique_ptr<StringLocalizerBase> localizer)
	{
		_localizer = std::move(localizer);

		if(_localizer)
		{
			_localizer->SetCulture(_culture);
		}
	}

	const StringLocalizerBase& Application::GetLocalizer() const
	{
		if(_localizer == nullptr)
		{
			throw std::runtime_error("Localizaer is not set");
		}

		return *_localizer;
	}

	void Application::Run()
	{		
		if(_isRunning)
		{
			return;
		}

		_isRunning = true;
		Window.Show();
		RunApp();
		Window.Hide();
	}

	void Application::Shutdown()
	{
		_isRunning = false;
		OnShutdown();
	}

	void Application::HandleEvents(Scene& scene)
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
							WindowStateEventArgs args(WindowState::Maximized);
							Window.OnWindowStateChanged(args);
							break;
						}
						
						case SDL_WINDOWEVENT_MINIMIZED:
						{
							WindowStateEventArgs args(WindowState::Minimized);
							Window.OnWindowStateChanged(args);
							break;
						}

						case SDL_WINDOWEVENT_RESTORED:
						{
							WindowStateEventArgs args(WindowState::Normal);
							Window.OnWindowStateChanged(args);
							break;
						}

						case SDL_WINDOWEVENT_SHOWN:
						{
							WindowVisibilityEventArgs args(true);
							Window.OnVisibilityChanged(args);
							break;
						}

						case SDL_WINDOWEVENT_HIDDEN:
						{
							WindowVisibilityEventArgs args(false);
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
						.State = ButtonState::Pressed,
						.Key = e.key.keysym
					};
					scene.OnKeyDown(args);
					break;
				}

				case SDL_KEYUP:
				{
					KeyEventArgs args =
					{
						.State = ButtonState::Released,
						.Key = e.key.keysym
					};
					scene.OnKeyUp(args);
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
					scene.OnTextEditing(args);
					break;
				}

				case SDL_TEXTINPUT:
				{
					TextInputEventArgs args(e.text.text);
					scene.OnTextInput(args);
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
					scene.OnTextEditing(args);
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
					scene.OnMouseMove(args);
					break;
				}

				case SDL_MOUSEBUTTONDOWN:
				{
					MouseButtonEventArgs args =
					{
						.Button = static_cast<MouseButton>(e.button.button),
						.State = ButtonState::Pressed,
						.ClicksNumber = e.button.clicks
					};
					scene.OnMouseDown(args);
					break;
				}

				case SDL_MOUSEBUTTONUP:
				{
					MouseButtonEventArgs args =
					{
						.Button = static_cast<MouseButton>(e.button.button),
						.State = ButtonState::Released,
						.ClicksNumber = e.button.clicks
					};
					scene.OnMouseUp(args);
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
					scene.OnMouseWheelChanged(args);
					break;
				}

				default:
					break;
			}
		}
	}

	void Application::RunApp()
	{
		OnRun();

		auto renderer = Window.GetSDLRenderer();

		RenderContext context;
		context.SetBlendMode(SDL_BLENDMODE_BLEND);

		Stopwatch frameStopwatch, processStopwatch;
		processStopwatch.Start();

		while(_isRunning)
		{
			frameStopwatch.Restart();

			auto scene = SceneManager.GetNextScene();
			if(scene == nullptr)
			{
				Shutdown();
				break;
			}

			HandleEvents(*scene);

			scene->Process(processStopwatch.Elapsed());
			processStopwatch.Restart();

			if(Window.IsVisible() || Window.IsRenderableWhenMinimized)
			{
				if(scene->NeedsRendering())
				{
					scene->Render(context);
				}

				SDL_RenderPresent(renderer);				
			}

			if(_fpsLimit != UnlimitedFps)
			{				
				auto frameTime = frameStopwatch.Elapsed();

				if(_frameTimeLimit > frameTime)
				{
					SleepFor(_frameTimeLimit - frameTime);
				}
			}

			_fpsCounter.Update(frameStopwatch.Elapsed());
		}
	}

	void Application::OnRun()
	{
		Started.TryInvoke(*this);
	}

	void Application::OnShutdown()
	{
		Stopped.TryInvoke(*this);
	}
}