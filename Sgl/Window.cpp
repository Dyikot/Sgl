#include "Window.h"

namespace Sgl
{
    constexpr static auto DefaultTitle = "Window";
    constexpr static auto DefaultWidth = 1280;
    constexpr static auto DefaultHeight = 720;
    constexpr static auto DefaultPosition = Point(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    Window::Window() noexcept:
        _window(SDL_CreateWindow(DefaultTitle, DefaultPosition.x, DefaultPosition.y,
                                 DefaultWidth, DefaultHeight, SDL_WINDOW_HIDDEN)),
        _renderer(SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED))
    {
        Log::PrintSDLErrorIf(_window == nullptr);
        Log::PrintSDLErrorIf(_renderer == nullptr);

        SetLogicalSize(Size(DefaultWidth, DefaultHeight));
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    }

    Window::~Window() noexcept
    {
        SDL_DestroyRenderer(_renderer);
        SDL_DestroyWindow(_window);
    }

    void Window::SetWidth(size_t value) noexcept
    {
        SDL_SetWindowSize(_window, value, GetHeight());
    }

    size_t Window::GetWidth() const noexcept
    {
        int width = 0;
        SDL_GetWindowSize(_window, &width, nullptr);
        return width;
    }

    void Window::SetHeight(size_t value) noexcept
    {
        SDL_SetWindowSize(_window, GetWidth(), value);
    }

    size_t Window::GetHeight() const noexcept
    {
        int height = 0;
        SDL_GetWindowSize(_window, nullptr, &height);
        return height;
    }

    void Window::SetLogicalSize(Size size) noexcept
    {
        SDL_RenderSetLogicalSize(_renderer, size.Width, size.Height);
    }

    Size Window::GetLogicalSize() const noexcept
    {
        int width = 0, height = 0;
        SDL_RenderGetLogicalSize(_renderer, &width, &height);
        return Size(width, height);
    }

    void Window::SetMaxSize(Size size) noexcept
    {
        SDL_SetWindowMaximumSize(_window, size.Width, size.Height);
    }

    Size Window::GetMaxSize() const noexcept
    {
        int width = 0, height = 0;
        SDL_GetWindowMaximumSize(_window, &width, &height);
        return Size(width, height);
    }

    void Window::SetMinSize(Size size) noexcept
    {        
        SDL_SetWindowMinimumSize(_window, size.Width, size.Height);
    }

    Size Window::GetMinSize() const noexcept
    {
        int width = 0, height = 0;
        SDL_GetWindowMinimumSize(_window, &width, &height);
        return Size(width, height);
    }

    void Window::SetTitle(std::string_view value) noexcept
    {
        SDL_SetWindowTitle(_window, value.data());
    }

    std::string_view Window::GetTitle() const noexcept
    {
        return std::string_view(SDL_GetWindowTitle(_window));
    }

    void Window::SetPosition(SDL_Point value) noexcept
    {
        SDL_SetWindowPosition(_window, value.x, value.y);
    }

    SDL_Point Window::GetPosition() const noexcept
    {
        SDL_Point position = {};
        SDL_GetWindowPosition(_window, &position.x, &position.y);
        return position;
    }

    void Window::SetIcon(std::string_view path)
    {
        _icon = Surface(path);
        SDL_SetWindowIcon(_window, _icon.value());
    }

    const std::optional<Surface>& Window::GetIcon() const
    {
        return _icon;
    }

    void Window::SetDisplayMode(DisplayMode displayMode)
    {
        switch(displayMode)
        {
            case DisplayMode::Window: 
                SDL_SetWindowFullscreen(_window, 0);
                break;

            case DisplayMode::BorderlessWindow:
                SDL_SetWindowBordered(_window, SDL_FALSE);
                break;

            case DisplayMode::Fullscreen:
                SDL_SetWindowFullscreen(_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                break;

            default:
                throw std::invalid_argument("Selected display mode does not exist!");
        }
    }

    DisplayMode Window::GetDisplayMode() const noexcept
    {
        auto flags = SDL_GetWindowFlags(_window);
        auto state = SDL_WindowFlags(flags & (SDL_WINDOW_FULLSCREEN | SDL_WINDOW_BORDERLESS));

        switch(state)
        {
            case SDL_WINDOW_FULLSCREEN:
                return DisplayMode::Fullscreen;

            case SDL_WINDOW_BORDERLESS:
                return DisplayMode::BorderlessWindow;

            default:
                return DisplayMode::Window;
        }
    }

    void Window::SetVSync(bool value)
    {
        if(!SDL_RenderSetVSync(_renderer, static_cast<int>(value)))
        {
            _isVSyncEnable = value;
        }
        else
        {
            Log::PrintSDLError();
        }
    }

    void Window::SetResize(bool value)
    {
        SDL_SetWindowResizable(_window, static_cast<SDL_bool>(value));
    }    

    void Window::SetWindowState(WindowState state) noexcept
    {
        switch(state)
        {
            case Sgl::WindowState::Normal:
                SDL_RestoreWindow(_window);
                break;

            case Sgl::WindowState::Minimized:
                SDL_MinimizeWindow(_window);
                break;

            case Sgl::WindowState::Maximized:
                SDL_MaximizeWindow(_window);
                break;

            default:
                break;
        }
    }

    WindowState Window::GetWindowState() const noexcept
    {
        auto flags = SDL_GetWindowFlags(_window);
        auto state = SDL_WindowFlags(flags & (SDL_WINDOW_MINIMIZED | SDL_WINDOW_MAXIMIZED));

        switch(state)
        {
            case SDL_WINDOW_MINIMIZED: 
                return WindowState::Minimized;

            case SDL_WINDOW_MAXIMIZED: 
                return WindowState::Maximized;

            default:
                return WindowState::Normal;
        }
    }

    void Window::EnableVSync() noexcept
    {
        SetVSync(true); 
    }

    void Window::DisableVSync() noexcept
    {
        SetVSync(false);
    }

    bool Window::IsVSyncEnable() const
    {
        return _isVSyncEnable;
    }

    void Window::EnableResizable() noexcept
    {
        SetResize(true);
    }

    void Window::DisableResizable() noexcept
    {
        SetResize(false);
    }

    bool Window::IsResizable() const
    {
        return SDL_GetWindowFlags(_window) & SDL_WINDOW_RESIZABLE;
    }

    void Window::Show()
    {
        SDL_ShowWindow(_window);
    }

    void Window::Hide()
    {
        SDL_HideWindow(_window);
    }

    bool Window::IsVisible() const
    {
        return !(SDL_GetWindowFlags(_window) & (SDL_WINDOW_HIDDEN | SDL_WINDOW_MINIMIZED));
    }    
}