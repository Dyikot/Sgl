#include "Window.h"

namespace Sgl
{
    Window::Window(Application& app) noexcept:
        application(app),
        _this(SDL_CreateWindow(_defaultTitle, _defaultPosition.x, _defaultPosition.y,
                               _defaultWidth, _defaultHeight, SDL_WINDOW_HIDDEN)),
        _renderer(SDL_CreateRenderer(_this, -1, SDL_RENDERER_ACCELERATED))
    {
        PrintSDLErrorIf(_this == nullptr);
        SetLogicalSize(_defaultWidth, _defaultHeight);
    }

    Window::~Window() noexcept
    {
        SDL_DestroyRenderer(_renderer);
        SDL_DestroyWindow(_this);
    }

    void Window::SetWidth(size_t value) noexcept
    {
        SDL_SetWindowSize(_this, value, GetHeight());
    }

    void Window::SetHeight(size_t value) noexcept
    {
        SDL_SetWindowSize(_this, GetWidth(), value);
    }

    void Window::SetLogicalSize(size_t width, size_t height) noexcept
    {
        SDL_RenderSetLogicalSize(_renderer, width, height);
    }

    void Window::SetMaxSize(size_t width, size_t height) noexcept
    {
        SDL_SetWindowMaximumSize(_this, width, height);
    }

    void Window::SetMinSize(size_t width, size_t height) noexcept
    {        
        SDL_SetWindowMinimumSize(_this, width, height);
    }

    void Window::SetTitle(std::string_view value) noexcept
    {
        SDL_SetWindowTitle(_this, value.data());
    }

    void Window::SetPosition(SDL_Point value) noexcept
    {
        SDL_SetWindowPosition(_this, value.x, value.y);
    }

    void Window::SetIcon(std::string_view path)
    {
        _icon = Surface(path);
        SDL_SetWindowIcon(_this, _icon.value());
    }

    void Window::SetDisplayMode(DiplayMode displayMode)
    {
        switch(displayMode)
        {
            case DiplayMode::Window: 
                SDL_SetWindowFullscreen(_this, 0); break;
            case DiplayMode::BorderlessWindow:
                SDL_SetWindowFullscreen(_this, SDL_WINDOW_FULLSCREEN_DESKTOP); break;
            case DiplayMode::Fullscreen:
                SDL_SetWindowFullscreen(_this, SDL_WINDOW_FULLSCREEN); break;
            default:
                throw std::invalid_argument("Selected display mode does not exist!");
        }
    }

    size_t Window::GetWidth() const noexcept
    {
        int width = 0;
        SDL_GetWindowSize(_this, &width, nullptr);
        return width;
    }

    size_t Window::GetHeight() const noexcept
    {
        int height = 0;
        SDL_GetWindowSize(_this, nullptr, &height);
        return height;
    }

    size_t Window::GetMaxWidth() const noexcept
    {
        int width = 0;
        SDL_GetWindowMaximumSize(_this, &width, nullptr);
        return width;
    }

    size_t Window::GetMaxHeight() const noexcept
    {
        int height = 0;
        SDL_GetWindowMaximumSize(_this, nullptr, &height);
        return height;
    }

    size_t Window::GetMinWidth() const noexcept
    {
        int width = 0;
        SDL_GetWindowMinimumSize(_this, &width, nullptr);
        return width;
    }

    size_t Window::GetMinHeight() const noexcept
    {
        int height = 0;
        SDL_GetWindowMinimumSize(_this, nullptr, &height);
        return height;
    }

    size_t Window::GetLogicalWidth() const noexcept
    {
        int width = 0;
        SDL_RenderGetLogicalSize(_renderer, &width, nullptr);
        return width;
    }

    size_t Window::GetLogicalHeight() const noexcept
    {
        int height = 0;
        SDL_RenderGetLogicalSize(_renderer, nullptr, &height);
        return height;
    }

    std::string_view Window::GetTitle() const noexcept
    {        
        return std::string_view(SDL_GetWindowTitle(_this));
    }

    SDL_Point Window::GetPosition() const noexcept
    {
        SDL_Point position = {};
        SDL_GetWindowPosition(_this, &position.x, &position.y);
        return position;
    }

    WindowState Window::GetWindowState() const noexcept
    {
        auto flags = SDL_GetWindowFlags(_this);
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

    RenderContext Window::GetRenderContext() const noexcept
    {
        return RenderContext(_renderer);
    }

    void Window::Show()
    {
        SDL_ShowWindow(_this);
    }

    void Window::Hide()
    {
        SDL_HideWindow(_this);
    }

    bool Window::IsVisible() const
    {
        return !(SDL_GetWindowFlags(_this) & (SDL_WINDOW_HIDDEN | SDL_WINDOW_MINIMIZED));
    }

    void Window::EnableVsync()
    {
        if(!SDL_RenderSetVSync(_renderer, 1))
        {
            _vsyncEnabled = true;
        }
        else
        {
            PrintSDLError();
        }
    }

    void Window::EnableResize()
    {
        SDL_SetWindowResizable(_this, SDL_TRUE);
    }

    void Window::DisableVsync()
    {
        if(!SDL_RenderSetVSync(_renderer, 0))
        {
            _vsyncEnabled = false;
        }
        else
        {
            PrintSDLError();
        }
    }

    void Window::DisableResize()
    {
        SDL_SetWindowResizable(_this, SDL_FALSE);
    }

    bool Window::IsResizable() const
    {
        return SDL_GetWindowFlags(_this) & SDL_WINDOW_RESIZABLE;
    }

    bool Window::IsVsyncEnable() const
    {
        return _vsyncEnabled;
    }
}