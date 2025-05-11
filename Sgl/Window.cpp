#include "Window.h"

namespace Sgl
{
    constexpr auto DefaultTitle = "Window";
    constexpr auto DefaultWidth = 1280;
    constexpr auto DefaultHeight = 720;
    constexpr auto DefaultPosition = SDL_Point(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    Window::Window() noexcept:
        _widnow(SDL_CreateWindow(DefaultTitle, DefaultPosition.x, DefaultPosition.y,
                                 DefaultWidth, DefaultHeight, SDL_WINDOW_HIDDEN)),
        _renderer(SDL_CreateRenderer(_widnow, -1, SDL_RENDERER_ACCELERATED))
    {
        PrintSDLErrorIf(_widnow == nullptr);
        SetLogicalSize(DefaultWidth, DefaultHeight);
    }

    Window::~Window() noexcept
    {
        SDL_DestroyRenderer(_renderer);
        SDL_DestroyWindow(_widnow);
    }

    void Window::SetWidth(size_t value) noexcept
    {
        SDL_SetWindowSize(_widnow, value, GetHeight());
    }

    void Window::SetHeight(size_t value) noexcept
    {
        SDL_SetWindowSize(_widnow, GetWidth(), value);
    }

    void Window::SetLogicalSize(size_t width, size_t height) noexcept
    {
        SDL_RenderSetLogicalSize(_renderer, width, height);
    }

    void Window::SetMaxSize(size_t width, size_t height) noexcept
    {
        SDL_SetWindowMaximumSize(_widnow, width, height);
    }

    void Window::SetMinSize(size_t width, size_t height) noexcept
    {        
        SDL_SetWindowMinimumSize(_widnow, width, height);
    }

    void Window::SetTitle(std::string_view value) noexcept
    {
        SDL_SetWindowTitle(_widnow, value.data());
    }

    void Window::SetPosition(SDL_Point value) noexcept
    {
        SDL_SetWindowPosition(_widnow, value.x, value.y);
    }

    void Window::SetIcon(std::string_view path)
    {
        _icon = Surface(path);
        SDL_SetWindowIcon(_widnow, _icon.value());
    }

    void Window::SetDisplayMode(DiplayMode displayMode)
    {
        switch(displayMode)
        {
            case DiplayMode::Window: 
                SDL_SetWindowFullscreen(_widnow, 0);
                break;

            case DiplayMode::BorderlessWindow:
                SDL_SetWindowFullscreen(_widnow, SDL_WINDOW_FULLSCREEN_DESKTOP);
                break;

            case DiplayMode::Fullscreen:
                SDL_SetWindowFullscreen(_widnow, SDL_WINDOW_FULLSCREEN);
                break;

            default:
                throw std::invalid_argument("Selected display mode does not exist!");
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
            PrintSDLError();
        }
    }

    void Window::SetResize(bool value)
    {
        SDL_SetWindowResizable(_widnow, static_cast<SDL_bool>(value));
    }

    size_t Window::GetWidth() const noexcept
    {
        int width = 0;
        SDL_GetWindowSize(_widnow, &width, nullptr);
        return width;
    }

    size_t Window::GetHeight() const noexcept
    {
        int height = 0;
        SDL_GetWindowSize(_widnow, nullptr, &height);
        return height;
    }

    size_t Window::GetMaxWidth() const noexcept
    {
        int width = 0;
        SDL_GetWindowMaximumSize(_widnow, &width, nullptr);
        return width;
    }

    size_t Window::GetMaxHeight() const noexcept
    {
        int height = 0;
        SDL_GetWindowMaximumSize(_widnow, nullptr, &height);
        return height;
    }

    size_t Window::GetMinWidth() const noexcept
    {
        int width = 0;
        SDL_GetWindowMinimumSize(_widnow, &width, nullptr);
        return width;
    }

    size_t Window::GetMinHeight() const noexcept
    {
        int height = 0;
        SDL_GetWindowMinimumSize(_widnow, nullptr, &height);
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

    std::string Window::GetTitle() const noexcept
    {        
        return std::string(SDL_GetWindowTitle(_widnow));
    }

    SDL_Point Window::GetPosition() const noexcept
    {
        SDL_Point position = {};
        SDL_GetWindowPosition(_widnow, &position.x, &position.y);
        return position;
    }

    WindowState Window::GetWindowState() const noexcept
    {
        auto flags = SDL_GetWindowFlags(_widnow);
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

    Renderer Window::GetRenderer() const noexcept
    {
        return _renderer;
    }

    void Window::EnableVSync() noexcept
    {
        SetVSync(true); 
    }

    void Window::DisableVSync() noexcept
    {
        SetVSync(false);
    }

    void Window::EnableResizable() noexcept
    {
        SetResize(true);
    }

    void Window::DisableResizable() noexcept
    {
        SetResize(false);
    }

    void Window::Show()
    {
        SDL_ShowWindow(_widnow);
    }

    void Window::Hide()
    {
        SDL_HideWindow(_widnow);
    }

    bool Window::IsVisible() const
    {
        return !(SDL_GetWindowFlags(_widnow) & (SDL_WINDOW_HIDDEN | SDL_WINDOW_MINIMIZED));
    }

    bool Window::IsResizable() const
    {
        return SDL_GetWindowFlags(_widnow) & SDL_WINDOW_RESIZABLE;
    }

    bool Window::IsVSyncEnable() const
    {
        return _isVSyncEnable;
    }
}