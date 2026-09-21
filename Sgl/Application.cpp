#include "Application.h"

#include <ranges>
#include <cassert>
#include <SDL3/SDL_init.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_mixer/SDL_mixer.h>

#include "Window.h"
#include "Base/Threading/Dispatcher.h"
#include "Base/Exceptions.h"
#include "Base/Logging.h"
#include "Base/Time/Stopwatch.h"
#include "Base/Media/Audio.h"
#include "UIElements/Buttons/CheckBox.h"
#include "UIElements/Buttons/RadioButton.h"
#include "UIElements/Buttons/SwithButton.h"

using namespace Sgl::UIElements;

namespace Sgl
{
    namespace
    {
        MouseButton FromSDLButton(SDL_MouseButtonFlags button)
        {
            switch(button)
            {
                case SDL_BUTTON_LEFT:   return MouseButton::Left;
                case SDL_BUTTON_MIDDLE: return MouseButton::Middle;
                case SDL_BUTTON_RIGHT:  return MouseButton::Right;
                case SDL_BUTTON_X1:     return MouseButton::XButton1;
                case SDL_BUTTON_X2:     return MouseButton::XButton2;
                default: throw Exception("Unsupported mouse button");
            }
        }

        MouseWheelDirection FromSDLWheelDirection(SDL_MouseWheelDirection direction)
        {
            switch(direction)
            {
                case SDL_MOUSEWHEEL_NORMAL:     return MouseWheelDirection::Normal;
                case SDL_MOUSEWHEEL_FLIPPED:    return MouseWheelDirection::Flipped;
                default: throw Exception("Unsupported mouse wheel direction");
            }
        }

        constexpr double MaxFrameTime = 1e3 / 60.0;
        constexpr uint32_t SDL_EVENT_SHUTDOWN = 0x8000;
    }

	Application::Application() noexcept
	{
		_current = this;

        if(!SDL_Init(SDL_INIT_VIDEO))
        {
            Logging::LogError("Unable to initialize SDL: {}", SDL_GetError());
        }

        if(!TTF_Init())
        {
            Logging::LogError("Unable to initialize TTF: {}", SDL_GetError());
        }

        if(!MIX_Init())
        {
            Logging::LogError("Unable to initialize MIX: {}", SDL_GetError());
        }

        SDL_SetHint(SDL_HINT_RENDER_LINE_METHOD, "2");

        SDL_RegisterEvents(1);
        SetThemeVariant(ThemeVariant::System);
        AddDefaultStyles();
        RegisterDefaultServices();
	}

	Application::~Application()
	{        
        MainWindow = nullptr;
        delete _services;
        _current = nullptr;
        MIX_Quit();
		TTF_Quit();
		SDL_Quit();
	}

    void Application::SetThemeVariant(ThemeVariant value)
    {
        if(_themeVariant == value)
        {
            return;
        }

        ThemeMode themeMode;

        switch(value)
        {
            case ThemeVariant::Light:
                themeMode = ThemeMode::Light; 
                break;

            case ThemeVariant::Dark:
                themeMode = ThemeMode::Dark;
                break;

            case ThemeVariant::System:
                themeMode = GetSystemThemeMode();
                break;

            default:
                throw Exception("ThemeVariant with index '{}' does not exist", static_cast<int>(value));
        }

        _themeVariant = value;
        Resources.SetCurrentTheme(themeMode);
        OnThemeVariantChanged();
    }

    void Application::WithStyles(const Action<const StyleCollection&>& action) const
    {
        action(Styles);
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
            Logging::LogInfo("FPS: {}", _fps);

            _fps = 0;
            s.Restart();
        }
    }

    Window* Application::GetWindow(uint32_t id)
    {
        auto it = std::ranges::find(_windowsById, id, &std::pair<uint32_t, Window*>::first);
        return it != _windowsById.end() ? it->second : nullptr;
    }

    void Application::Run()
	{		
		if(_isRunning)
		{
            Logging::LogWarning("Application already running");
			return;
		}        

		OnStarted();

		while(_isRunning)
		{
            uint64_t start = SDL_GetPerformanceCounter();

			HandleInputEvents();
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
		Started.Invoke(*this);

        if(MainWindow)
        {
            MainWindow->Show();            
        }
	}

	void Application::OnStopped()
	{
		Stopped.Invoke(*this);
	}

    void Application::OnThemeVariantChanged()
    {
        ThemeVariantChanged.Invoke(*this);

        for(auto& window : _activeWindows)
        {
            window->RecalculateStyle();
        }
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
                    if(auto window = GetWindow(e.window.windowID))
                    {
                        MouseMoveEventArgs args(e.button.x, e.button.y);

                        window->OnMouseMove(args);
                    }

                    break;
                }

                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                {
                    if(auto window = _focusedWindow)
                    {
                        MouseClickEventArgs args =
                        {
                            e.button.x,
                            e.button.y,
                            FromSDLButton(e.button.button),
                            e.button.clicks
                        };

                        window->OnMouseDown(args);
                    }

                    break;
                }

                case SDL_EVENT_MOUSE_BUTTON_UP:
                {
                    if(auto window = _focusedWindow)
                    {
                        MouseClickEventArgs args =
                        {
                            e.button.x,
                            e.button.y,
                            FromSDLButton(e.button.button),
                            e.button.clicks
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
                            e.wheel.mouse_x,
                            e.wheel.mouse_y,
                            e.wheel.integer_x,
                            e.wheel.integer_y,
                            FromSDLWheelDirection(e.wheel.direction)
                        };

                        window->OnMouseWheelChanged(args);
                    }

                    break;
                }

                case SDL_EVENT_KEY_DOWN:
                {
                    if(auto window = _focusedWindow)
                    {
                        KeyModifier modifier = e.key.mod & ~SDL_KMOD_NUM;
                        KeyCode key = SDL_GetKeyFromScancode(e.key.scancode, modifier, false);
                        KeyEventArgs args(key, modifier);

                        window->OnKeyDown(args);
                    }

                    break;
                }

                case SDL_EVENT_KEY_UP:
                {                   
                    if(auto window = _focusedWindow)
                    {
                        KeyModifier modifier = e.key.mod & ~SDL_KMOD_NUM;
                        KeyCode key = SDL_GetKeyFromScancode(e.key.scancode, modifier, false);
                        KeyEventArgs args(key, modifier);

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
                    if(auto window = GetWindow(e.window.windowID))
                    {
                        window->OnShown();
                    }

                    break;
                }

                case SDL_EVENT_WINDOW_HIDDEN:
                {
                    if(auto window = GetWindow(e.window.windowID))
                    {
                        if(window->_isClosing)
                        {
                            window->OnClosed();
                        }
                    }

                    break;
                }

                /*case SDL_EVENT_WINDOW_EXPOSED:
                {
                    if(auto window = GetWindow(e.window.windowID))
                    {
                        window->RenderCore();
                    }

                    break;
                }*/

                case SDL_EVENT_WINDOW_MOVED:
                {
                    if(auto window = GetWindow(e.window.windowID))
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
                    if(auto window = GetWindow(e.window.windowID))
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
                    if(auto window = GetWindow(e.window.windowID))
                    {
                        WindowStateChangedEventArgs args(WindowState::Minimized);
                        window->OnWindowStateChanged(args);
                    }

                    break;
                }

                case SDL_EVENT_WINDOW_MAXIMIZED:
                {
                    if(auto window = GetWindow(e.window.windowID))
                    {
                        WindowStateChangedEventArgs args(WindowState::Maximized);
                        window->OnWindowStateChanged(args);
                    }

                    break;
                }

                case SDL_EVENT_WINDOW_RESTORED:
                {
                    if(auto window = GetWindow(e.window.windowID))
                    {
                        WindowStateChangedEventArgs args(WindowState::Normal);
                        window->OnWindowStateChanged(args);
                    }

                    break;
                }

                case SDL_EVENT_WINDOW_MOUSE_ENTER:
                {
                    if(auto window = GetWindow(e.window.windowID))
                    {
                        window->OnMouseEnter();
                    }

                    break;
                }

                case SDL_EVENT_WINDOW_MOUSE_LEAVE:
                {
                    if(auto window = GetWindow(e.window.windowID))
                    {
                        window->OnMouseLeave();
                    }

                    break;
                }

                case SDL_EVENT_WINDOW_FOCUS_GAINED:
                {
                    if(auto window = GetWindow(e.window.windowID))
                    {
                        window->OnActivated();
                        _focusedWindow = window;
                    }

                    break;
                }

                case SDL_EVENT_WINDOW_FOCUS_LOST:
                {
                    if(auto window = GetWindow(e.window.windowID))
                    {
                        window->OnDeactivated();
                        _focusedWindow = nullptr;
                    }

                    break;
                }

                case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                {
                    if(auto window = GetWindow(e.window.windowID))
                    {
                        CancelEventArgs args { .Cancel = false };
                        window->OnClosing(args);

                        if(args.Cancel)
                        {
                            break;
                        }

                        window->Hide();

                        bool shutdown = !_activeWindows.empty() &&
                                        window == MainWindow.Get() &&
                                        ShutdownMode == ShutdownMode::OnMainWindowClose;

                        if(shutdown)
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

    void Application::AddDefaultStyles()
    {
        Styles.New().OfType<CheckBox>().On("checked")
            .Set(CheckBox::BackgroundProperty, ImageSource(AssetId::CheckBox));

        Styles.New().OfType<RadioButton>().On("checked")
            .Set(CheckBox::BackgroundProperty, ImageSource(AssetId::RadioButton));

        Styles.New().OfType<SwitchButon>().On("checked")
            .Set(CheckBox::BackgroundProperty, ImageSource(AssetId::SwitchButtonOn));
    }

    void Application::RegisterDefaultServices()
    {
        _services->Add<Media::Mixer>();
    }

    void Application::PushSDLUserEvent(uint32_t type)
    {
        SDL_Event e;
        e.user = SDL_UserEvent { .type = type };
        SDL_PushEvent(&e);
    }

    void Application::AddWindow(Window& window)
    {
        _windowsById.emplace_back(window.GetId(), &window);
    }

    void Application::RemoveWindow(Window& window)
    {
        auto it = std::ranges::find(_windowsById, window.GetId(), &std::pair<uint32_t, Window*>::first);

        if(it != _windowsById.end())
        {
            auto index = std::distance(_windowsById.begin(), it);
            _windowsById.erase(_windowsById.begin() + index);
        }
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
        if(std::erase(_activeWindows, &window) > 0)
        {
            window.SetParent(nullptr);
            window.OnDetachedFromLogicalTree();
        }
    }

    std::string StringLocalizer::operator()(std::string_view key) const
    {
        return App->Localization.GetLocalizedString(key);
    }
}