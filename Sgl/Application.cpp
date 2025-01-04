#include "Application.h"
#include "Render/Shapes.h"

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

	const Cursor& Application::GetCursor() const
	{
		return _activeCursor;
	}

	void Application::Run(Window& window)
	{
		if(_isRunning)
		{
			return;
		}

		_window = &window;
		OnStartup(EventArgs());

		while(_isRunning)
		{
			if(!window.Scenes.Any())
			{
				Shutdown();
				continue;
			}

			_stopwatch.Restart();

			HandleEvents(window.Scenes.Current());

			if(window.Scenes.IsCurrentClosed())
			{
				continue;
			}

			window.Scenes.Current()->Process(_stopwatch.Elapsed());
			window.Render();

			if(_maxFrameRate)
			{
				SDL_Delay(_maxFrameTime.value() - _stopwatch.Elapsed());
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
		_isRunning = false;
	}

	void Application::OnStartup(const EventArgs& e)
	{
		_isRunning = true;
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

	void Application::HandleEvents(Scene* scene)
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

				case SDL_KEYDOWN:
				{
					scene->OnKeyDown(
						KeyEventArgs
						{
							.State = static_cast<ButtonState>(e.key.state),
							.Key = e.key.keysym
						}
					);

					break;
				}

				case SDL_KEYUP:
				{				
					scene->OnKeyUp(
						KeyEventArgs
						{
							.State = static_cast<ButtonState>(e.key.state),
							.Key = e.key.keysym
						}
					);

					break;
				}

				case SDL_TEXTEDITING:
				{
					scene->OnTextChanged(
						TextChangedEventArgs
						{
							.Text = e.edit.text,
							.SelectionLength = static_cast<size_t>(e.edit.length),
							.SelectionStart = e.edit.start
						}
					);

					break;
				}

				case SDL_TEXTEDITING_EXT:
				{
					scene->OnTextChanged(
						TextChangedEventArgs
						{
							.Text = e.editExt.text,
							.SelectionLength = static_cast<size_t>(e.editExt.length),
							.SelectionStart = e.editExt.start
						}
					);
					SDL_free(e.editExt.text);

					break;
				}

				case SDL_TEXTINPUT:
				{
					scene->OnTextInput(
						TextInputEventArgs
						{
							.Text = e.text.text
						}
					);

					break;
				}

				case SDL_MOUSEBUTTONDOWN:
				{
					scene->OnMouseDown(
						MouseButtonEventArgs
						{
							.Button = static_cast<MouseButton>(e.button.button),
							.State = static_cast<ButtonState>(e.button.state),
							.ClicksCount = e.button.clicks,
							.Position =
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
							.Button = static_cast<MouseButton>(e.button.button),
							.State = static_cast<ButtonState>(e.button.state),
							.ClicksCount = e.button.clicks,
							.Position =
							{
								.x = static_cast<float>(e.button.x),
								.y = static_cast<float>(e.button.y)
							}
						}
					);

					break;
				}

				case SDL_MOUSEMOTION:
				{
					scene->OnMouseMove(
						MouseButtonEventArgs
						{
							.Position =
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
							.Position =
							{
								.x = static_cast<float>(e.button.x),
								.y = static_cast<float>(e.button.y)
							},
							.ScrolledHorizontally = e.wheel.preciseX,
							.ScrolledVertically = e.wheel.preciseY,
							.Direction = SDL_MouseWheelDirection(e.wheel.direction)
						}
					);

					break;
				}
			}
		}
	}
}