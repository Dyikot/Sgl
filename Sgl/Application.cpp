#include "Application.h"

#include <SDL3/SDL_log.h>
#include <SDL3/SDL_init.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "Base/Time/Timer.h"
#include "Base/Time/Delay.h"
#include "Base/Localization/CSVParser.h"
#include "Base/Localization/StringLocalizer.h"

namespace Sgl
{
	Application::Application() noexcept
	{
		_current = this;

        if(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
        {
            SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        }

        if(!TTF_Init())
        {
            SDL_Log("Unable to initialize TTF: %s", SDL_GetError());
        }

        _systemTheme = QuerySystemTheme();
        auto displayMode = SDL_GetCurrentDisplayMode(SDL_GetPrimaryDisplay());

        if(displayMode == nullptr)
        {
            SDL_Log("Unable to get display mode: %s", SDL_GetError());
        }
        else
        {            
            if(displayMode->refresh_rate != 0.f)
            {
                _delayDuration = TimeSpan(1.f / displayMode->refresh_rate * 1e9f);
            }
        }
	}

	Application::~Application()
	{        
        _mainWindow.reset();
		TTF_Quit();
		SDL_Quit();
	}

    void Application::SetThemeMode(ThemeMode value)
    {
        _themeMode = value;
        OnThemeModeChanged(value);
    }

    ThemeMode Application::GetThemeMode() const
    {
        return _themeMode;
    }

    SystemTheme Application::GetSystemTheme() const
    {
        return _systemTheme;        
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
        return _mainWindow.get();
    }

    const std::vector<Window*> Application::GetWindows() const noexcept
    {
        return _activeWindows;
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
        _stopwatch.Start();

		while(_isRunning)
		{
			HandleEvents();

            for(auto window : _activeWindows)
            {
			    window->ProcessCore();
            }

            for(auto window : _activeWindows)
            {
			    window->RenderCore();
            }

            Delay();
		}

        _stopwatch.Reset();
		_mainWindow->Close();
		OnStop();
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

    void Application::OnThemeModeChanged(ThemeMode theme)
    {
        ThemeModeChanged.TryInvoke(*this, theme);
    }

    void Application::OnSystemThemeChanged(SystemTheme theme)
    {
        SystemThemeChanged.TryInvoke(*this, theme);
    }

    SystemTheme Application::QuerySystemTheme() const
    {
        auto theme = SDL_GetSystemTheme();

        if(theme == SDL_SystemTheme::SDL_SYSTEM_THEME_UNKNOWN)
        {
            SDL_Log("System theme unknown");
            return SystemTheme::Light;
        }

        return SystemTheme(theme);
    }

    void Application::Delay()
    {
        auto elapsed = _stopwatch.Elapsed();
        _stopwatch.Restart();

        if(elapsed < _delayDuration)
        {
            SleepFor(_delayDuration - elapsed);
        }
    }

    void Application::HandleEvents()
	{
        SDL_Event e;
		while(SDL_PollEvent(&e))
		{
            switch(e.type)
            {
                case SDL_EVENT_QUIT:
                {
                    Shutdown();
                    break;
                }

                case SDL_EVENT_SYSTEM_THEME_CHANGED:
                {
                    _systemTheme = QuerySystemTheme();
                    OnSystemThemeChanged(_systemTheme);
                    break;
                };

                case SDL_EVENT_WINDOW_SHOWN:
                {
                    auto window = GetWindowById(e.window.windowID);
                    window->OnShow();
                    break;
                }

                case SDL_EVENT_WINDOW_HIDDEN:
                {
                    auto window = GetWindowById(e.window.windowID);
                    window->OnHide();
                    break;
                }

                case SDL_EVENT_WINDOW_EXPOSED:
                {
                    break;
                }

                case SDL_EVENT_WINDOW_MOVED:
                {
                    auto window = GetWindowById(e.window.windowID);
                    Point args(e.window.data1, e.window.data2);
                    window->OnPositionChanged(args);
                    break;
                }

                case SDL_EVENT_WINDOW_RESIZED:
                {
                    auto window = GetWindowById(e.window.windowID);
                    Size args(e.window.data1, e.window.data2);
                    window->OnWindowSizeChanged(args);
                    break;
                }

                case SDL_EVENT_WINDOW_MINIMIZED:
                {
                    auto window = GetWindowById(e.window.windowID);
                    WindowState args = WindowState::Minimized;
                    window->OnWindowStateChanged(args);
                    break;
                }

                case SDL_EVENT_WINDOW_MAXIMIZED:
                {
                    auto window = GetWindowById(e.window.windowID);
                    WindowState args  = WindowState::Maximized;
                    window->OnWindowStateChanged(args);
                    break;
                }

                case SDL_EVENT_WINDOW_RESTORED:
                {
                    auto window = GetWindowById(e.window.windowID);
                    WindowState args = WindowState::Normal;
                    window->OnWindowStateChanged(args);
                    break;
                }

                case SDL_EVENT_WINDOW_MOUSE_ENTER:
                {
                    auto window = GetWindowById(e.window.windowID);
                    window->OnMouseEnter();
                    break;
                }

                case SDL_EVENT_WINDOW_MOUSE_LEAVE:
                {
                    auto window = GetWindowById(e.window.windowID);
                    window->OnMouseLeave();
                    break;
                }

                case SDL_EVENT_WINDOW_FOCUS_GAINED:
                {
                    auto window = GetWindowById(e.window.windowID);
                    window->OnActivated();
                    _focusedWindow = window;
                    break;
                }

                case SDL_EVENT_WINDOW_FOCUS_LOST:
                {                    
                    auto window = GetWindowById(e.window.windowID);
                    window->OnDeactivated();
                    _focusedWindow = nullptr;
                    break;
                }

                case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                {
                    auto window = GetWindowById(e.window.windowID);
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

                case SDL_EVENT_KEY_DOWN:
                {
                    KeyEventArgs args =
                    {
                        .State = ButtonState::Pressed,
                        .Key = e.key.key,
                        .Modifier = e.key.mod
                    };
                    _focusedWindow->OnKeyDown(args);
                    break;
                }

                case SDL_EVENT_KEY_UP:
                {
                    KeyEventArgs args =
                    {
                        .State = ButtonState::Released,
                        .Key = e.key.key,
                        .Modifier = e.key.mod
                    };
                    _focusedWindow->OnKeyUp(args);
                    break;
                }

                case SDL_EVENT_TEXT_EDITING:
                {
                    TextEditingEventArgs args
                    {
                        .Text = e.edit.text,
                        .SelectionStart = e.edit.start,
                        .SelectionLength = e.edit.length
                    };
                    _focusedWindow->OnTextEditing(args);
                    break;
                }

                case SDL_EVENT_TEXT_INPUT:
                {
                    TextInputEventArgs args(e.text.text);
                    _focusedWindow->OnTextInput(args);
                    break;
                }

                case SDL_EVENT_MOUSE_MOTION:
                {
                    auto window = GetWindowById(e.window.windowID);
                    MouseEventArgs args(FPoint(e.button.x, e.button.y));
                    window->OnMouseMove(args);
                    break;
                }

                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                {
                    MouseButtonEventArgs args =
                    {
                        .Button = MouseButton(e.button.button),
                        .State = ButtonState::Pressed,
                        .ClicksNumber = e.button.clicks
                    };
                    _focusedWindow->OnMouseDown(args);
                    break;
                }

                case SDL_EVENT_MOUSE_BUTTON_UP:
                {
                    MouseButtonEventArgs args =
                    {
                        .Button = MouseButton(e.button.button),
                        .State = ButtonState::Released,
                        .ClicksNumber = e.button.clicks
                    };
                    _focusedWindow->OnMouseUp(args);
                    break;
                }

                case SDL_EVENT_MOUSE_WHEEL:
                {
                    MouseWheelEventArgs args =
                    {
                        .Position =
                        {
                            .x = e.button.x,
                            .y = e.button.y
                        },
                        .ScrolledHorizontally = e.wheel.x,
                        .ScrolledVertically = e.wheel.y,
                        .Direction = MouseWheelDirection(e.wheel.direction)
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
        _windows[window->_id] = window;
    }

    void Application::AddActiveWindow(Window* window)
    {
        if(std::ranges::find(_activeWindows, window) == _activeWindows.end())
        {
            _activeWindows.push_back(window);
        }
    }

    void Application::RemoveWindow(Window* window)
    {
        RemoveActiveWindow(window);
        _windows.erase(window->_id);
    }

    void Application::RemoveActiveWindow(Window* window)
    {
        std::erase(_activeWindows, window);
    }

    Window* Application::GetWindowById(int id)
    {
        return _windows[id];
    }
}