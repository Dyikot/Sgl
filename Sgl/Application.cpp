#include "Application.h"

#include <cassert>
#include <SDL3/SDL_init.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "Base/Time/Timer.h"
#include "Base/Threading/Dispatcher.h"
#include "Base/Threading/Time.h"
#include "Base/Exceptions.h"
#include "Base/Logger.h"
#include "Input/UserEvents.h"

namespace Sgl
{
	Application::Application() noexcept:
        _timeSheduler(TimeSheduler::Current())
	{
		_current = this;

        if(!SDL_Init(SDL_INIT_VIDEO))
        {
            Logger::LogError("Unable to initialize SDL: {}", SDL_GetError());
        }

        if(!TTF_Init())
        {
            Logger::LogError("Unable to initialize TTF: {}", SDL_GetError());
        }

        SDL_RegisterEvents(UserEventsNumber);
        SetThemeVariant(ThemeVariant::System);
	}

	Application::~Application()
	{        
        MainWindow = nullptr;
		TTF_Quit();
		SDL_Quit();
	}

    void Application::SetThemeVariant(ThemeVariant value)
    {
        _themeVariant = value;
        ThemeVariantChangedEventArgs args(value);
        OnThemeModeChanged(args);
    }

    void Application::SetCulture(const std::string& value)
	{
		_culture = value;

		if(_localizer)
		{
			_localizer->SetCulture(_culture);
		}
	}

	void Application::SetLocalizer(const Ref<StringLocalizerBase>& localizer)
	{
		_localizer = localizer;

		if(_localizer)
		{
			_localizer->SetCulture(_culture);
		}
	}

    void Application::Run()
	{		
		if(_isRunning)
		{
            Logger::LogWarning("Application already running");
			return;
		}        

		OnStarted();
		
		while(_isRunning)
		{
			HandleEvents();
            _timeSheduler.Process();
            UIThread.ProcessTasks();

            for(auto window : _activeWindows)
            {
			    window->Process();
            }

            for(auto window : _activeWindows)
            {
			    window->RenderCore();
            }  

            Delay();
		}
		
		OnStopped();
	}

	void Application::Shutdown()
	{
        if(!_isRunning)
        {
            return;
        }

        PushSDLUserEvent(SDL_EVENT_SHUTDOWN);
	}

	void Application::OnStarted()
	{
        _isRunning = true;
		Started(*this);

        if(MainWindow)
        {
            MainWindow->Show();            
        }

        _stopwatch.Start();
	}

	void Application::OnStopped()
	{
        _stopwatch.Reset();
		Stopped(*this);
	}

    void Application::OnThemeModeChanged(ThemeVariantChangedEventArgs e)
    {
        switch(e.Theme)
        {
            case ThemeVariant::Light:
            {
                _themeMode = ThemeMode::Light;
                break;
            }

            case ThemeVariant::Dark:
            {
                _themeMode = ThemeMode::Dark;
                break;
            }

            case ThemeVariant::System:
            {
                _themeMode = GetSystemThemeMode();
                break;
            }
        }

        ThemeVariantChanged(*this, e);
    }

    ThemeMode Application::GetSystemThemeMode() const
    {
        switch(SDL_GetSystemTheme())
        {
            case SDL_SYSTEM_THEME_UNKNOWN:
            case SDL_SYSTEM_THEME_LIGHT:
                return ThemeMode::Light;        

            case SDL_SYSTEM_THEME_DARK:
                return ThemeMode::Dark;
        }
    }

    static constexpr TimeSpan MaxDelay = TimeSpan(1e9 / 60.0);    

    void Application::Delay()
    {
        auto elapsed = _stopwatch.Elapsed();
        _stopwatch.Restart();       

        if(elapsed < MaxDelay)
        {
            SDL_DelayNS((MaxDelay - elapsed).ToNanoseconds());
        }
    }

    void Application::HandleEvents()
	{
        SDL_Event e;
		while(SDL_PollEvent(&e))
		{
            switch(e.type)
            {    
                case SDL_EVENT_MOUSE_MOTION:
                {
                    MouseMoveEventArgs args
                    {
                        .X = e.button.x,
                        .Y = e.button.y
                    };

                    auto window = GetWindowById(e.window.windowID);
                    window->OnMouseMove(args);

                    break;
                }

                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                {
                    MouseButtonEventArgs args =
                    {
                        .Button = MouseButton(e.button.button),
                        .ClicksNumber = e.button.clicks
                    };

                    if(auto window = _focusedWindow)
                    {
                        window->OnMouseDown(args);
                    }

                    break;
                }

                case SDL_EVENT_MOUSE_BUTTON_UP:
                {
                    MouseButtonEventArgs args =
                    {
                        .Button = MouseButton(e.button.button),
                        .ClicksNumber = e.button.clicks
                    };

                    if(auto window = _focusedWindow)
                    {
                        window->OnMouseUp(args);
                    }

                    break;
                }

                case SDL_EVENT_MOUSE_WHEEL:
                {
                    MouseWheelEventArgs args =
                    {
                        .X = e.wheel.mouse_x,
                        .Y = e.wheel.mouse_y,
                        .ScrolledByX = e.wheel.integer_x,
                        .ScrolledByY = e.wheel.integer_y,
                        .Direction = MouseWheelDirection(e.wheel.direction)
                    };

                    //Logger::LogInfo("MouseWheel: X = {}, Y = {}, ScrollByX = {}, ScrollByY = {}", args.X, args.Y, args.ScrolledByX, args.ScrolledByY);

                    if(auto window = _focusedWindow)
                    {
                        window->OnMouseWheelChanged(args);
                    }

                    break;
                }

                case SDL_EVENT_KEY_DOWN:
                {
                    KeyEventArgs args =
                    {
                        .Key = e.key.key,
                        .Modifier = e.key.mod & ~SDL_KMOD_NUM
                    };

                    if(auto window = _focusedWindow)
                    {
                        window->OnKeyDown(args);
                    }

                    break;
                }

                case SDL_EVENT_KEY_UP:
                {                   
                    KeyEventArgs args =
                    {
                        .Key = e.key.key,
                        .Modifier = e.key.mod & ~SDL_KMOD_NUM
                    };

                    if(auto window = _focusedWindow)
                    {
                        window->OnKeyUp(args);
                    }

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

                    if(auto window = _focusedWindow)
                    {
                        window->OnTextEditing(args);
                    }

                    break;
                }

                case SDL_EVENT_TEXT_INPUT:
                {
                    TextInputEventArgs args(e.text.text);

                    if(auto window = _focusedWindow)
                    {
                        window->OnTextInput(args);
                    }

                    break;
                }                
                
                case SDL_EVENT_WINDOW_HIDDEN:
                {
                    auto window = GetWindowById(e.window.windowID);    

                    if(window->_isClosing)
                    {
                        window->OnClosed();
                    }

                    break;
                }

                case SDL_EVENT_WINDOW_MOVED:
                {
                    WindowPositionChangedEventArgs args
                    {
                        .X = e.window.data1,
                        .Y = e.window.data2
                    };

                    auto window = GetWindowById(e.window.windowID);
                    window->OnPositionChanged(args);

                    break;
                }

                case SDL_EVENT_WINDOW_RESIZED:
                {
                    WindowSizeChangedEventArgs args
                    {
                        .Width = e.window.data1,
                        .Height = e.window.data2
                    };

                    auto window = GetWindowById(e.window.windowID);
                    window->OnWindowSizeChanged(args);

                    break;
                }

                case SDL_EVENT_WINDOW_MINIMIZED:
                {
                    WindowStateChangedEventArgs args(WindowState::Minimized);

                    auto window = GetWindowById(e.window.windowID);
                    window->OnWindowStateChanged(args);

                    break;
                }

                case SDL_EVENT_WINDOW_MAXIMIZED:
                {
                    WindowStateChangedEventArgs args(WindowState::Maximized);

                    auto window = GetWindowById(e.window.windowID);
                    window->OnWindowStateChanged(args);

                    break;
                }

                case SDL_EVENT_WINDOW_RESTORED:
                {
                    WindowStateChangedEventArgs args(WindowState::Normal);

                    auto window = GetWindowById(e.window.windowID);
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
                    CancelEventArgs args {};

                    auto window = GetWindowById(e.window.windowID);
                    window->OnClosing(args);                    

                    if(args.Cancel)
                    {
                        break;                    
                    }

                    window->Hide();

                    bool hasMultipleWindows = _activeWindows.size() > 0;
                    bool isMainWindow = window == MainWindow.Get();

                    if(hasMultipleWindows && isMainWindow && ShutdownMode == ShutdownMode::OnMainWindowClose)
                    {
                        PushSDLUserEvent(SDL_EVENT_SHUTDOWN);
                    }                    

                    break;
                }

                case SDL_EVENT_QUIT:
                {
                    if(ShutdownMode != ShutdownMode::OnExplicitShutdown)
                    {
                        PushSDLUserEvent(SDL_EVENT_SHUTDOWN);
                    }

                    break;
                }

                case SDL_EVENT_SHUTDOWN:
                {
                    _isRunning = false;
                    break;
                }                           

                default:
                    break;
            }
		}
	}

    void Application::PushSDLUserEvent(unsigned int type)
    {
        SDL_Event e;
        e.user = SDL_UserEvent { .type = type };
        SDL_PushEvent(&e);
    }

    void Application::AddWindow(Window& window)
    {
        _windows.push_back(&window);
        _windowsWithId.emplace_back(window._id, &window);
    }

    void Application::RemoveWindow(Window& window)
    {
        std::erase(_windows, &window);
        _windowsWithId.erase(std::ranges::find(_windowsWithId, &window, &WindowWithId::Window));
    }

    void Application::AttachWindow(Window& window)
    {
        if(std::ranges::find(_activeWindows, &window) == _activeWindows.end())
        {
            _activeWindows.push_back(&window);            
            window.SetParent(this);
            window.OnAttachedToLogicalTree();
        }
    }    

    void Application::DetachWindow(Window& window)
    {
        std::erase(_activeWindows, &window);
        window.SetParent(nullptr);
        window.OnDetachedFromLogicalTree();
    }

    Window* Application::GetWindowById(SDL_WindowID windowId)
    {
        for(auto& [id, window] : _windowsWithId)
        {
            if(id == windowId)
            {
                return window;
            }
        }

        return nullptr;
    }
}