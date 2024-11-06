#include "Window.h"
#include "Application.h"

namespace Sgl
{
    Window::Window() noexcept: 
        Window(WindowProperties{})
    {}

    Window::Window(const WindowProperties & properties) noexcept:
        _width(properties.Width),
        _height(properties.Height),
        SDLWindow(SDL_CreateWindow(properties.Title.data(),
                                   properties.Position.x, properties.Position.y,
                                   properties.Width, properties.Height,
                                   properties.Flags)),
        Renderer(SDL_CreateRenderer(SDLWindow, -1, SDL_RENDERER_ACCELERATED))
    {
        if(SDLWindow == nullptr)
        {
            std::cout << SDL_GetError() << '\n';
        }

        if(Renderer == nullptr)
        {
            std::cout << SDL_GetError() << '\n';
        }

        SDL_SetRenderDrawBlendMode(Renderer, SDL_BLENDMODE_BLEND);
        SetLogicalSize(properties.LogicalWidth, properties.LogicalHeight);
    }

    Window::~Window() noexcept
    {
        SDL_FreeSurface(_icon);
        SDL_DestroyRenderer(Renderer);
        SDL_DestroyWindow(SDLWindow);
    }

    void Window::SetWidth(size_t width) noexcept
    {
        _width = width;
        OnSizeChanged({ .Width = _width, .Height = _height });
    }

    void Window::SetHeight(size_t height) noexcept
    {
        _height = height;
        OnSizeChanged({ .Width = _width, .Height = _height });
    }

    void Window::SetLogicalSize(size_t width, size_t height)
    {
        SDL_RenderSetLogicalSize(Renderer, width, height);
    }

    void Window::SetMaxSize(size_t width, size_t height)
    {
        SDL_SetWindowMaximumSize(SDLWindow, width, height);
    }

    void Window::SetTitle(std::string_view value)
    {
        SDL_SetWindowTitle(SDLWindow, value.data());
    }

    void Window::SetPosition(SDL_Point value)
    {
        SDL_SetWindowPosition(SDLWindow, value.x, value.y);
    }

    void Window::SetIcon(std::string_view path)
    {
        SDL_FreeSurface(_icon);
        _icon = IMG_Load(path.data());
        SDL_SetWindowIcon(SDLWindow, _icon);
    }

    void Window::SetDisplayMode(DiplayMode displayMode)
    {
        switch(displayMode)
        {
            case Sgl::Window::DiplayMode::Window: 
                SDL_SetWindowFullscreen(SDLWindow, 0); break;
            case Sgl::Window::DiplayMode::BorderlessWindow:
                SDL_SetWindowFullscreen(SDLWindow, SDL_WINDOW_FULLSCREEN_DESKTOP); break;
            case Sgl::Window::DiplayMode::Fullscreen:
                SDL_SetWindowFullscreen(SDLWindow, SDL_WINDOW_FULLSCREEN); break;
        }
    }

    std::pair<size_t, size_t> Window::GetLogicalSize() const
    {
        int width, height;
        SDL_RenderGetLogicalSize(Renderer, &width, &height);
        return { width, height };
    }

    SDL_Point Window::GetPosition() const
    {
        SDL_Point position;
        SDL_GetWindowPosition(SDLWindow, &position.x, &position.y);
        return position;
    }

    void Window::Show()
    {
        SDL_ShowWindow(SDLWindow);
    }

    void Window::Hide()
    {
        SDL_HideWindow(SDLWindow);
    }

    void Window::Close()
    {
        Scenes.Clear();
    }

    void Window::Render() const
    {        
        if(Scenes.Empty() || !IsVisible())
        {
            return;
        }
        
        Scenes.Current()->Process();
        Scenes.Current()->OnRender();
        SDL_RenderPresent(Renderer);
    }    

    bool Window::IsVisible() const
    {
        return !(SDL_GetWindowFlags(SDLWindow) & (SDL_WINDOW_MINIMIZED | SDL_WINDOW_HIDDEN));
    }

    void Window::OnSizeChanged(const SizeChangedEventArgs& e)
    {       
        if(SizeChanged)
        {
            SizeChanged(this, e);
        }

        SDL_SetWindowSize(SDLWindow, _width, _height);
    }
}