#include "Application.h"

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
	}

	Application::~Application() noexcept
	{
		TTF_Quit();
		IMG_Quit();
		Mix_Quit();
		SDL_Quit();
	}

	void Application::SetMaxFrameRate(uint32_t value)
	{
		_maxFrameRate = value > MaxFrameRate? MaxFrameRate : value;
	}

	void Application::Run(Window& window)
	{
		MainWindow = &window;

		OnStartup(EventArgs());

		while(!window.Scenes.Empty())
		{
			auto currentScene = window.Scenes.Current();
			_start = SDL_GetPerformanceCounter();

			for(SDL_Event* e = nullptr; SDL_PollEvent(e);)
			{
				switch(e->type)
				{
					case SDL_QUIT: 
					{
						Shutdown();
						break;
					}

					case SDL_WINDOWEVENT:
					{
						window.OnStateChanged();
						break;
					}

					case SDL_KEYDOWN:
					{
						currentScene->OnKeyDown(
							KeyEventArgs
							{
								.IsDown = true,
								.IsUp = false,
								.Key = e->key.keysym
							}
						);

						break;
					}

					case SDL_KEYUP:
					{
						currentScene->OnKeyUp(
							KeyEventArgs
							{
								.IsDown = false,
								.IsUp = true,
								.Key = e->key.keysym
							}
						);

						break;
					}

					case SDL_TEXTEDITING:
					{
						currentScene->OnTextChanged(
							TextChangedEventArgs
							{
								.Text = e->edit.text,
								.SelectionLength = static_cast<size_t>(e->edit.length),
								.SelectionStart = e->edit.start
							}
						);

						break;
					}

					case SDL_TEXTEDITING_EXT:
					{
						currentScene->OnTextChanged(
							TextChangedEventArgs
							{
								.Text = e->editExt.text,
								.SelectionLength = static_cast<size_t>(e->editExt.length),
								.SelectionStart = e->editExt.start
							}
						);
						SDL_free(e->editExt.text);

						break;
					}

					case SDL_TEXTINPUT:
					{
						currentScene->OnTextInput(
							TextInputEventArgs
							{
								.Text = e->text.text
							}
						);

						break;
					}

					case SDL_MOUSEBUTTONDOWN:
					{
						currentScene->OnMouseDown(
							MouseButtonEventArgs
							{
								.Button = ToMouseButton(e->button.button),
								.ButtonState = ToMouseButtonState(e->button.state),
								.ClickCount = e->button.clicks,
								.Position = { e->button.x, e->button.y }
							}
						);

						break;
					}

					case SDL_MOUSEBUTTONUP:
					{
						currentScene->OnMouseUp(
							MouseButtonEventArgs
							{
								.Button = ToMouseButton(e->button.button),
								.ButtonState = ToMouseButtonState(e->button.state),
								.ClickCount = e->button.clicks,
								.Position = { e->button.x, e->button.y }
							}
						);

						break;
					}

					case SDL_MOUSEMOTION:
					{
						currentScene->OnMouseMove(
							MouseButtonEventArgs
							{
								.Position = { e->motion.x, e->motion.y }
							}
						);

						break;
					}

					case SDL_MOUSEWHEEL:
					{
						currentScene->OnMouseWheel(
							MouseWheelEventArgs
							{
								.Position = { e->wheel.mouseX, e->wheel.mouseY },
								.ScrolledHorizontally = e->wheel.preciseX,
								.ScrolledVertically = e->wheel.preciseY,
								.Direction = SDL_MouseWheelDirection(e->wheel.direction)
							}
						);

						break;
					}
				}
			}

			if(currentScene->IsClosed())
			{
				window.Scenes.Pop();
				continue;
			}
			currentScene->Process(ElapsedMs());
			window.Render();

			if(_maxFrameRate.has_value())
			{
				SDL_Delay(MaxFrameTimeMs() - ElapsedMs());
			}
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
		if(MainWindow != nullptr)
		{
			MainWindow->Close();
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