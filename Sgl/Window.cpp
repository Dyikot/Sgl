#include "Window.h"
#include "Application.h"

namespace Sgl
{
    Window::Window() noexcept:
        Window("Window", SDL_Point{ 0, 0 }, 1280, 720, SDL_WINDOW_SHOWN)
    {}

    Window::Window(std::string_view title,
                   SDL_Point position, 
                   size_t width, 
                   size_t height, 
                   SDL_WindowFlags flags) noexcept:
        _width(width),
        _height(height),
        _sdlWindow(SDL_CreateWindow(title.data(), position.x, position.y,
                                    width, height, flags)),
        _renderContext(SDL_CreateRenderer(_sdlWindow, -1, SDL_RENDERER_ACCELERATED))
    {
        if(_sdlWindow == nullptr)
        {
            PrintSDLError();
        }

        if(_renderContext == nullptr)
        {
            PrintSDLError();
        }

        _renderContext.SetBlendMode(SDL_BLENDMODE_BLEND);
        SetLogicalSize(width, height);
    }

    void Window::SetWidth(size_t width) noexcept
    {
        _width = width;
        OnSizeChanged(SizeChangedEventArgs{ .Width= _width, .Height = _height });
    }

    void Window::SetHeight(size_t height) noexcept
    {
        _height = height;
        OnSizeChanged(SizeChangedEventArgs{ .Width = _width, .Height = _height });
    }

    void Window::SetLogicalSize(size_t width, size_t height)
    {
        SDL_RenderSetLogicalSize(_renderContext, width, height);
    }

    void Window::SetMaxSize(size_t width, size_t height)
    {
        SDL_SetWindowMaximumSize(_sdlWindow, width, height);
    }

    void Window::SetTitle(std::string_view value)
    {
        SDL_SetWindowTitle(_sdlWindow, value.data());
    }

    void Window::SetPosition(SDL_Point value)
    {
        SDL_SetWindowPosition(_sdlWindow, value.x, value.y);
    }

    void Window::SetIcon(Surface&& icon)
    {
        _icon = std::move(icon);
        SDL_SetWindowIcon(_sdlWindow, _icon.value());
    }

    void Window::SetIcon(const Surface& icon)
    {
        _icon = icon;
        SDL_SetWindowIcon(_sdlWindow, _icon.value());
    }

    void Window::SetDisplayMode(DiplayMode displayMode)
    {
        switch(displayMode)
        {
            case DiplayMode::Window: 
                SDL_SetWindowFullscreen(_sdlWindow, 0); break;
            case DiplayMode::BorderlessWindow:
                SDL_SetWindowFullscreen(_sdlWindow, SDL_WINDOW_FULLSCREEN_DESKTOP); break;
            case DiplayMode::Fullscreen:
                SDL_SetWindowFullscreen(_sdlWindow, SDL_WINDOW_FULLSCREEN); break;
        }
    }

    std::pair<size_t, size_t> Window::GetLogicalSize() const
    {
        int width, height;
        SDL_RenderGetLogicalSize(_renderContext, &width, &height);
        return { width, height };
    }

    SDL_Point Window::GetPosition() const
    {
        SDL_Point position;
        SDL_GetWindowPosition(_sdlWindow, &position.x, &position.y);
        return position;
    }

    WindowState Window::GetWindowState() const
    {
        const auto flags = SDL_GetWindowFlags(_sdlWindow);

        if(!(flags & (SDL_WINDOW_MINIMIZED | SDL_WINDOW_MAXIMIZED)))
        {
            return WindowState::Normal;
        }

        if(flags & SDL_WINDOW_MAXIMIZED)
        {
            return WindowState::Maximized;
        }
        
        if(flags & SDL_WINDOW_MINIMIZED)
        {
            return WindowState::Minimized;
        }
    }

    void Window::Render()
    {
        if(!IsVisible())
        {
            return;
        }

        Scenes.Current()->OnRender(GetRenderContext());
        SDL_RenderPresent(_renderContext);
    }

    void Window::Show()
    {
        SDL_ShowWindow(_sdlWindow);
    }

    void Window::Hide()
    {
        SDL_HideWindow(_sdlWindow);
    }

    void Window::Close()
    {
        Scenes.Clear();
    }

    bool Window::IsVisible() const
    {
        return SDL_GetWindowFlags(_sdlWindow) & (SDL_WINDOW_MINIMIZED | SDL_WINDOW_HIDDEN);
    }

    void Window::EnableVsync()
    {
        if(!SDL_RenderSetVSync(_renderContext, 1))
        {
            _isVsyncEnable = true;
        }
        else
        {
            PrintSDLError();
        }
    }

    void Window::DisableVsync()
    {
        if(!SDL_RenderSetVSync(_renderContext, 0))
        {
            _isVsyncEnable = false;
        }
        else
        {
            PrintSDLError();
        }
    }

    void Window::OnSizeChanged(const SizeChangedEventArgs& e)
    {       
        if(SizeChanged)
        {
            SizeChanged(this, e);
        }

        SDL_SetWindowSize(_sdlWindow, _width, _height);
    }

    void Window::OnStateChanged(const EventArgs& e)
    {
        if(StateChanged)
        {
            StateChanged(this, e);
        }
    }
}