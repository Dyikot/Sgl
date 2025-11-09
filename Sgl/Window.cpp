#include "Window.h"
#include "Base/Log.h"
#include "Base/Math.h"
#include "Base/Time/Delay.h"
#include "Application.h"

namespace Sgl
{
    constexpr auto DefaultTitle = "Window";
    constexpr auto DefaultWidth = 1280;
    constexpr auto DefaultHeight = 720;
    constexpr auto DefaultPosition = Point(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    
    Window::Window() noexcept:
        _window(SDL_CreateWindow(DefaultTitle, DefaultPosition.x, DefaultPosition.y,
                                 DefaultWidth, DefaultHeight, SDL_WINDOW_HIDDEN)),
        _renderer(SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)),
        _context(_renderer)
    {
        Log::PrintSDLErrorIf(_window == nullptr);
        Log::PrintSDLErrorIf(_renderer == nullptr);

        _id = SDL_GetWindowID(_window);
        _context.SetBlendMode(SDL_BLENDMODE_BLEND);
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
        SetBackground(Colors::White);

        App->AddWindow(this);
    }

    Window::~Window()
    {
        App->RemoveWindow(this);
        SDL_DestroyRenderer(GetSDLRenderer());
        SDL_DestroyWindow(GetSDLWindow());
    }

    SDL_Window* Window::GetSDLWindow() const noexcept
    {
        return _window;
    }

    SDL_Renderer* Window::GetSDLRenderer() const noexcept
    {
        return _renderer;
    }

    int Window::GetId() const noexcept
    {
        return _id;
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

    void Window::SetSize(Size size) noexcept
    {
        SDL_SetWindowSize(_window, size.Width, size.Height);
    }

    Size Window::GetSize() const noexcept
    {
        int width = 0, height = 0;
        SDL_GetWindowSize(_window, &width, &height);
        return Size(width, height);
    }

    void Window::SetRenderSize(Size size) noexcept
    {
        SDL_RenderSetLogicalSize(_renderer, size.Width, size.Height);
    }

    Size Window::GetRenderSize() const noexcept
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

    void Window::SetPosition(Point value) noexcept
    {
        SDL_SetWindowPosition(_window, value.x, value.y);
    }

    Point Window::GetPosition() const noexcept
    {
        Point position = {};
        SDL_GetWindowPosition(_window, &position.x, &position.y);
        return position;
    }

    void Window::SetDisplayMode(WindowDisplayMode displayMode)
    {
        switch(displayMode)
        {
            case WindowDisplayMode::Window: 
                SDL_SetWindowFullscreen(_window, 0);
                break;

            case WindowDisplayMode::BorderlessWindow:
                SDL_SetWindowBordered(_window, SDL_FALSE);
                break;

            case WindowDisplayMode::Fullscreen:
                SDL_SetWindowFullscreen(_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                break;

            default:
                throw std::invalid_argument("Selected display mode does not exist!");
        }
    }

    WindowDisplayMode Window::GetDisplayMode() const noexcept
    {
        auto flags = SDL_GetWindowFlags(_window);
        auto state = SDL_WindowFlags(flags & (SDL_WINDOW_FULLSCREEN | SDL_WINDOW_BORDERLESS));

        switch(state)
        {
            case SDL_WINDOW_FULLSCREEN:
                return WindowDisplayMode::Fullscreen;

            case SDL_WINDOW_BORDERLESS:
                return WindowDisplayMode::BorderlessWindow;

            default:
                return WindowDisplayMode::Window;
        }
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

    void Window::SetIcon(Surface icon)
    {
        _icon = std::move(icon);
        SDL_SetWindowIcon(_window, _icon.ToSDLSurface());
    }

    Surface Window::GetIcon() const
    {
        return _icon;
    }

    void Window::SetResizable(bool value) noexcept
    {
        SDL_SetWindowResizable(_window, SDL_bool(value));
    }

    bool Window::IsResizable() const
    {
        return SDL_GetWindowFlags(_window) & SDL_WINDOW_RESIZABLE;
    }

    void Window::SetContent(Ref<UIElement> value)
    {
        if(_content)
        {
            _content->_parent = nullptr;
        }

        SetProperty(ContentProperty, _content, std::move(value));

        if(_content)
        {
            _content->_parent = this;
        }
    }

    Ref<UIElement> Window::GetContent() const 
    {
        return _content; 
    }

    void Window::Show()
    {
        SDL_ShowWindow(_window);
    }

    void Window::Hide()
    {
        SDL_HideWindow(_window);
    }

    void Window::Focus()
    {
        SDL_RaiseWindow(_window);
    }

    bool Window::IsVisible() const
    {
        return !(SDL_GetWindowFlags(_window) & (SDL_WINDOW_HIDDEN | SDL_WINDOW_MINIMIZED));
    }

    void Window::Render(RenderContext context)
    {
        switch(auto background = GetBackground(); background.GetType())
        {
            case BrushType::Color:
                context.SetBackground(background.AsColor()); break;
            case BrushType::Texture:
                context.DrawTexture(background.AsTexture()); break;
        }

        if(_content && _content->IsVisible())
        {
            _content->Render(context);
        }

        Renderable::Render(context);
    }

    void Window::Process(TimeSpan elapsed)
    {
        UpdateStyleAndLayout();
    }

    void Window::OnCursorChanged(const Cursor& cursor)
    {
        if(!(_content && _content->IsMouseOver()))
        {
            Cursor::Set(cursor);
        }
    }

    void Window::OnWindowStateChanged(WindowStateEventArgs& e)
    {
        WindowStateChanged.TryInvoke(*this, e);
    }

    void Window::OnVisibilityChanged(WindowVisibilityEventArgs& e)
    {
        VisibilityChanged.TryInvoke(*this, e);
    }

    void Window::OnPositionChanged(WindowPositionChangedEventArgs& e)
    {
        PositionChanged.TryInvoke(*this, e);
    }

    void Window::OnWindowSizeChanged(WindowSizeChangedEventArgs& e)
    {
        SizeChanged.TryInvoke(*this, e);
    }

    void Window::OnMouseMove(MouseEventArgs& e)
    {
        if(_content && _content->IsVisible())
        {
            auto& content = _content.GetValue();
            bool wasMouseOver = content._isMouseOver;
            bool isMouseOver = Math::IsPointInRect(e.Position, content._bounds);

            if(isMouseOver)
            {
                if(!wasMouseOver)
                {
                    content.OnMouseEnter(e);
                }

                content.OnMouseMove(e);
            }
            else if(wasMouseOver)
            {
                content.OnMouseLeave(e);
                Cursor::Set(GetCursor());
            }
        }
    }

    void Window::OnMouseDown(MouseButtonEventArgs& e)
    {
        if(_content && _content->IsMouseOver() && _content->IsVisible())
        {
            _content->OnMouseDown(e);
        }
    }

    void Window::OnMouseUp(MouseButtonEventArgs& e)
    {
        if(_content && _content->IsMouseOver() && _content->IsVisible())
        {
            _content->OnMouseUp(e);
        }
    }

    void Window::RenderCore()
    {
        if(IsVisible() || IsRenderableWhenMinimized)
        {
            if(NeedsRendering())
            {
                Render(_context);
            }

            SDL_RenderPresent(_renderer);
        }
    }

    void Window::ProcessCore()
    {
        Process(_stopwatch.Elapsed());
        _stopwatch.Restart();
    }

    void Window::UpdateStyleAndLayout()
    {
        if(!IsStyleValid())
        {
            ApplyStyle();
        }

        if(_content)
        {
            if(!_content->IsStyleValid())
            {
                _content->ApplyStyle();
            }

            if(!_content->IsMeasureValid())
            {
                auto [width, height] = GetSize();
                _content->Measure(FSize(width, height));
            }

            if(!_content->IsArrangeValid())
            {
                auto [width, height] = GetSize();
                _content->Arrange(FRect(0, 0, width, height));
            }
        }
    }
}