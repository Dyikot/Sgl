#include "Application.h"
#include "Base/Log.h"
#include "Base/Time/Timer.h"
#include "Base/Time/Delay.h"
#include "Base/Localization/CSVParser.h"
#include "Base/Localization/StringLocalizer.h"

namespace Sgl
{
    constexpr auto SleepDuration = TimeSpan::FromMilliseconds(100);

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
        _mainWindow.release();
		TTF_Quit();
		IMG_Quit();
		Mix_Quit();
		SDL_Quit();
	}

	void Application::SetCulture(const std::string& value)
	{
		_culture = value;

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

    void Application::SetMainWindow(std::unique_ptr<Window> value)
    {
        _mainWindow = std::move(value);
    }

    Window* Application::GetMainWindow() const
    {
        return _mainWindow != nullptr ? _mainWindow.get() : nullptr;
    }

    const std::vector<Window*> Application::GetWindows() const noexcept
    {
        return _windows;
    }

    void Application::Run()
	{		
		if(_isRunning)
		{
			return;
		}        

		OnRun();
		_isRunning = true;

        if(_mainWindow == nullptr)
        {
            throw std::runtime_error("MainWindow is null");
        }

		_mainWindow->Show();

		while(_isRunning)
		{
			HandleEvents();

            for(auto window : _windows)
            {
			    window->ProcessCore();
            }

            for(auto window : _windows)
            {
			    window->RenderCore();
            }

            if(_visibleWindows == 0)
            {
                SleepFor(SleepDuration);
            }
		}

		_mainWindow->Close();
		OnStop();
	}

    void Application::Run(std::unique_ptr<Window> window)
    {
        _mainWindow = std::move(window);
        Run();
    }

	void Application::Shutdown()
	{
		_isRunning = false;
	}    

	void Application::OnRun()
	{
		Started.TryInvoke(*this);
	}

	void Application::OnStop()
	{
		Stopped.TryInvoke(*this);
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
                    auto window = GetWindowById(e.window.windowID);

                    switch(e.window.event)
                    {
                        case SDL_WINDOWEVENT_SHOWN:
                        {
                            ++_visibleWindows;
                            WindowVisibilityEventArgs args(true);
                            window->OnVisibilityChanged(args);
                            break;
                        }

                        case SDL_WINDOWEVENT_HIDDEN:
                        {
                            --_visibleWindows;
                            WindowVisibilityEventArgs args(false);
                            window->OnVisibilityChanged(args);
                            break;
                        }

                        case SDL_WINDOWEVENT_EXPOSED:
                        {
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
                            window->OnPositionChanged(args);
                            break;
                        }

                        case SDL_WINDOWEVENT_RESIZED:
                        {
                            break;
                        }

                        case SDL_WINDOWEVENT_SIZE_CHANGED:
                        {
                            WindowSizeChangedEventArgs args =
                            {
                                .Width = static_cast<size_t>(e.window.data1),
                                .Height = static_cast<size_t>(e.window.data2)
                            };
                            window->OnWindowSizeChanged(args);
                            break;
                        }

                        case SDL_WINDOWEVENT_MINIMIZED:
                        {
                            --_visibleWindows;
                            WindowStateEventArgs args(WindowState::Minimized);
                            window->OnWindowStateChanged(args);
                            break;
                        }

                        case SDL_WINDOWEVENT_MAXIMIZED:
                        {
                            WindowStateEventArgs args(WindowState::Maximized);
                            window->OnWindowStateChanged(args);
                            break;
                        }                        

                        case SDL_WINDOWEVENT_RESTORED:
                        {
                            ++_visibleWindows;
                            WindowStateEventArgs args(WindowState::Normal);
                            window->OnWindowStateChanged(args);
                            break;
                        }

                        case SDL_WINDOWEVENT_ENTER:
                        {
                            window->OnMouseEnter();
                            break;
                        }

                        case SDL_WINDOWEVENT_LEAVE:
                        {
                            window->OnMouseLeave();
                            break;
                        }

                        case SDL_WINDOWEVENT_FOCUS_GAINED:
                        {
                            _focusedWindow = window;
                            window->OnActivated();
                            break;
                        }

                        case SDL_WINDOWEVENT_FOCUS_LOST:
                        {
                            window->OnDeactivated();
                            break;
                        }

                        case SDL_WINDOWEVENT_CLOSE:
                        {
                            window->OnClosing();

                            if(window == _mainWindow.get())
                            {
                                Shutdown();
                            }
                            else
                            {
                                window->Close();
                            }

                            window->OnClosed();

                            break;
                        }

                        case SDL_WINDOWEVENT_TAKE_FOCUS:
                        {
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

                    _focusedWindow->OnKeyDown(args);
                    break;
                }

                case SDL_KEYUP:
                {
                    KeyEventArgs args =
                    {
                        .State = ButtonState::Released,
                        .Key = e.key.keysym
                    };
                    _focusedWindow->OnKeyUp(args);
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
                    _focusedWindow->OnTextEditing(args);
                    break;
                }

                case SDL_TEXTINPUT:
                {
                    TextInputEventArgs args(e.text.text);
                    _focusedWindow->OnTextInput(args);
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
                    _focusedWindow->OnTextEditing(args);
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
                    _focusedWindow->OnMouseMove(args);
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
                    _focusedWindow->OnMouseDown(args);
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
                    _focusedWindow->OnMouseUp(args);
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
                    _focusedWindow->OnMouseWheelChanged(args);
                    break;
                }

                default:
                    break;
            }
		}
	}

    void Application::AddWindow(Window* window)
    {
        if(std::ranges::find(_windows, window) == _windows.end())
        {
            _windows.push_back(window);
        }
    }

    void Application::RemoveWindow(Window* window)
    {
        std::erase(_windows, window);
    }

    Window* Application::GetWindowById(int id)
    {
        for(auto window : _windows)
        {
            if(window->_id == id)
            {
                return window;
            }
        }

        return nullptr;
    }
}