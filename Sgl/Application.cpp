#include "Application.h"

#include <cassert>
#include <SDL3/SDL_init.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "Base/Threading/Dispatcher.h"
#include "Base/Threading/DelayDispatcher.h"
#include "Base/Exceptions.h"
#include "Base/Logger.h"
#include "Base/Time/Stopwatch.h"
#include "Input/SDLEvents.h"

namespace Sgl
{
    static constexpr size_t MaxWindowsNumber = 100;
    static constexpr double MaxFrameTime = 1e3 / 60.0;

	Application::Application() noexcept:
        _windows(MaxWindowsNumber)
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

        SDL_SetHint(SDL_HINT_RENDER_LINE_METHOD, "2");

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
        if(_themeVariant == value)
        {
            return;
        }

        _themeVariant = value;

        switch(value)
        {
            case ThemeVariant::Light:
                _themeMode = ThemeMode::Light; 
                break;

            case ThemeVariant::Dark:
                _themeMode = ThemeMode::Dark;
                break;

            case ThemeVariant::System:
                _themeMode = GetSystemThemeMode();
                break;

            default:
                throw Exception("ThemeVariant with index '{}' does not exist", static_cast<int>(value));
        }

        ThemeVariantChangedEventArgs args(value);
        OnThemeVariantChanged(args);
    }

    static inline double ToMilliseconds(uint64_t count)
    {
        static const double msMultiplier = 1e3 / SDL_GetPerformanceFrequency();
        return count * msMultiplier;
    }

    static void LogFps()
    {
        static constexpr TimeSpan _1s = TimeSpan::FromSeconds(1);
        static Stopwatch s = Stopwatch::StartNew();
        static int _fps = 0;
        _fps++;

        if(s.Elapsed() > _1s)
        {
            Logger::LogInfo("FPS: {}", _fps);

            _fps = 0;
            s.Restart();
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
            uint64_t start = SDL_GetPerformanceCounter();

			HandleInputEvents();
            DefaultDelayDispatcher.Process();
            UIThread.Process();

            for(auto window : _activeWindows)
            {
			    window->Process();
            }

            for(auto window : _activeWindows)
            {
			    window->RenderCore();
            }              
            
            double elapsedMs = ToMilliseconds(SDL_GetPerformanceCounter() - start);
            double delayMs = MaxFrameTime - elapsedMs;

            if(delayMs > 0)
            {            
                SDL_DelayNS(delayMs * 1e6);
            }

            //LogFps();
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
	}

	void Application::OnStopped()
	{
		Stopped(*this);
	}

    void Application::OnThemeVariantChanged(ThemeVariantChangedEventArgs e)
    {
        for(auto& window : _activeWindows)
        {
            window->ApplyStyle();
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

    void Application::HandleInputEvents()
	{
        SDL_Event e;
		while(SDL_PollEvent(&e))
		{
            switch(e.type)
            {    
                case SDL_EVENT_MOUSE_MOTION:
                {
                    if(auto window = _windows[e.window.windowID])
                    {
                        MouseMoveEventArgs args
                        {
                            .X = e.button.x,
                            .Y = e.button.y
                        };

                        window->OnMouseMove(args);
                    }

                    break;
                }

                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                {
                    if(auto window = _focusedWindow)
                    {
                        MouseButtonEventArgs args =
                        {
                            .Button = MouseButton(e.button.button),
                            .ClicksNumber = e.button.clicks
                        };

                        window->OnMouseDown(args);
                    }

                    break;
                }

                case SDL_EVENT_MOUSE_BUTTON_UP:
                {
                    if(auto window = _focusedWindow)
                    {
                        MouseButtonEventArgs args =
                        {
                            .Button = MouseButton(e.button.button),
                            .ClicksNumber = e.button.clicks
                        };

                        window->OnMouseUp(args);
                    }

                    break;
                }

                case SDL_EVENT_MOUSE_WHEEL:
                {
                    if(auto window = _focusedWindow)
                    {
                        MouseWheelEventArgs args =
                        {
                            .X = e.wheel.mouse_x,
                            .Y = e.wheel.mouse_y,
                            .ScrolledByX = e.wheel.integer_x,
                            .ScrolledByY = e.wheel.integer_y,
                            .Direction = MouseWheelDirection(e.wheel.direction)
                        };

                        window->OnMouseWheelChanged(args);
                    }

                    break;
                }

                case SDL_EVENT_KEY_DOWN:
                {
                    if(auto window = _focusedWindow)
                    {
                        KeyEventArgs args =
                        {
                            .Key = e.key.key,
                            .Modifier = e.key.mod & ~SDL_KMOD_NUM
                        };

                        window->OnKeyDown(args);
                    }

                    break;
                }

                case SDL_EVENT_KEY_UP:
                {                   
                    if(auto window = _focusedWindow)
                    {
                        KeyEventArgs args =
                        {
                            .Key = e.key.key,
                            .Modifier = e.key.mod & ~SDL_KMOD_NUM
                        };

                        window->OnKeyUp(args);
                    }

                    break;
                }

                case SDL_EVENT_TEXT_EDITING:
                {
                    if(auto window = _focusedWindow)
                    {
                        TextEditingEventArgs args
                        {
                            .Text = e.edit.text,
                            .SelectionStart = e.edit.start,
                            .SelectionLength = e.edit.length
                        };

                        window->OnTextEditing(args);
                    }

                    break;
                }

                case SDL_EVENT_TEXT_INPUT:
                {
                    if(auto window = _focusedWindow)
                    {
                        TextInputEventArgs args(e.text.text);
                        window->OnTextInput(args);
                    }

                    break;
                }                
                
                case SDL_EVENT_WINDOW_SHOWN:
                {
                    if(auto window = _windows[e.window.windowID])
                    {
                        window->OnShown();
                    }

                    break;
                }

                case SDL_EVENT_WINDOW_HIDDEN:
                {
                    if(auto window = _windows[e.window.windowID])
                    {
                        if(window->_isClosing)
                        {
                            window->OnClosed();
                        }
                    }

                    break;
                }

                case SDL_EVENT_WINDOW_MOVED:
                {
                    if(auto window = _windows[e.window.windowID])
                    {
                        WindowPositionChangedEventArgs args
                        {
                            .X = e.window.data1,
                            .Y = e.window.data2
                        };

                        window->OnPositionChanged(args);
                    }

                    break;
                }

                case SDL_EVENT_WINDOW_RESIZED:
                {
                    if(auto window = _windows[e.window.windowID])
                    {
                        WindowSizeChangedEventArgs args
                        {
                            .Width = e.window.data1,
                            .Height = e.window.data2
                        };

                        window->OnWindowSizeChanged(args);
                    }

                    break;
                }

                case SDL_EVENT_WINDOW_MINIMIZED:
                {
                    if(auto window = _windows[e.window.windowID])
                    {
                        WindowStateChangedEventArgs args(WindowState::Minimized);
                        window->OnWindowStateChanged(args);
                    }

                    break;
                }

                case SDL_EVENT_WINDOW_MAXIMIZED:
                {
                    if(auto window = _windows[e.window.windowID])
                    {
                        WindowStateChangedEventArgs args(WindowState::Maximized);
                        window->OnWindowStateChanged(args);
                    }

                    break;
                }

                case SDL_EVENT_WINDOW_RESTORED:
                {
                    if(auto window = _windows[e.window.windowID])
                    {
                        WindowStateChangedEventArgs args(WindowState::Normal);
                        window->OnWindowStateChanged(args);
                    }

                    break;
                }

                case SDL_EVENT_WINDOW_MOUSE_ENTER:
                {
                    if(auto window = _windows[e.window.windowID])
                    {
                        window->OnMouseEnter();
                    }

                    break;
                }

                case SDL_EVENT_WINDOW_MOUSE_LEAVE:
                {
                    if(auto window = _windows[e.window.windowID])
                    {
                        window->OnMouseLeave();
                    }

                    break;
                }

                case SDL_EVENT_WINDOW_FOCUS_GAINED:
                {
                    if(auto window = _windows[e.window.windowID])
                    {
                        window->OnActivated();
                        _focusedWindow = window;
                    }

                    break;
                }

                case SDL_EVENT_WINDOW_FOCUS_LOST:
                {
                    if(auto window = _windows[e.window.windowID])
                    {
                        window->OnDeactivated();
                        _focusedWindow = nullptr;
                    }

                    break;
                }

                case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                {
                    if(auto window = _windows[e.window.windowID])
                    {
                        CancelEventArgs args {};
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
        auto id = window._id;
        if(id >= _windows.size())
        {
            _windows.resize(_windows.size() + MaxWindowsNumber);
        }

        _windows[id] = &window;
    }

    void Application::RemoveWindow(Window& window)
    {
        _windows[window._id] = nullptr;
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
}