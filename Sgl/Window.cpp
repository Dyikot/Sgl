#include "Window.h"
#include <SDL3/SDL_log.h>
#include "Base/Time/Delay.h"
#include "Application.h"
#include "Render/BackgroundRenderer.h"
#include "Layout/LayoutHelper.h"

namespace Sgl
{
    static constexpr auto DefaultTitle = "Window";
    static constexpr auto DefaultWidth = 1280;
    static constexpr auto DefaultHeight = 720;
    static constexpr auto DefaultPosition = Point(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    
    Window::Window():
        _window(SDL_CreateWindow(DefaultTitle, DefaultWidth, DefaultHeight, SDL_WINDOW_HIDDEN)),
        _renderer(SDL_CreateRenderer(_window, nullptr)),
        _renderContext(_renderer)
    {
        if(_window == nullptr)
        {
            SDL_Log("Unable to create a window: %s", SDL_GetError());
        }
        else
        {
            if(App.Current() == nullptr)
            {
                throw std::runtime_error("Cannot create a window without an application class");
            }

            _id = SDL_GetWindowID(_window);
            App->AddWindow(this);
        }

        if(_renderer == nullptr)
        {
            SDL_Log("Unable to create a renderer: %s", SDL_GetError());
        }

        SetVisualRoot(this);
        SetBackground(Colors::White);
    }

    Window::~Window()
    {
        if(_content)
        {
            _content->SetParent(nullptr);
            _content->SetVisualRoot(nullptr);
            _content->OnDetachedFromElementsTree();
            _content = nullptr;
        }

        Close();
        App->RemoveWindow(this);
        SDL_DestroyRenderer(_renderer);
        SDL_DestroyWindow(_window);
    }

    SDL_Window* Window::GetSDLWindow() const noexcept
    {
        return _window;
    }

    SDL_Renderer* Window::GetRenderer() const noexcept
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
                SDL_SetWindowFullscreen(_window, false);
                SDL_SetWindowBordered(_window, true);
                break;

            case WindowDisplayMode::BorderlessWindow:
                SDL_SetWindowFullscreen(_window, false);
                SDL_SetWindowBordered(_window, false);
                break;

            case WindowDisplayMode::Fullscreen:
                SDL_SetWindowFullscreen(_window, true);
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

    void Window::SetIcon(const std::string& iconSource)
    {
        _icon = Surface(iconSource);

        if(!SDL_SetWindowIcon(_window, _icon.GetSDLSurface()))
        {
            SDL_Log("Unable to set a window icon: %s", SDL_GetError());
        }
    }

    const std::string& Window::GetIcon() const
    {
        return _iconSource;
    }

    void Window::SetResizable(bool value) noexcept
    {
        SDL_SetWindowResizable(_window, value);
    }

    bool Window::IsResizable() const
    {
        return SDL_GetWindowFlags(_window) & SDL_WINDOW_RESIZABLE;
    }

    void Window::SetAlwayOnTop(bool value)
    {
        SDL_SetWindowAlwaysOnTop(_window, value);
    }

    bool Window::IsAlwayOnTop() const
    {
        return SDL_GetWindowFlags(_window) & SDL_WINDOW_ALWAYS_ON_TOP;
    }

    void Window::SetOwner(Window* owner)
    {
        _owner = owner;

        if(owner != nullptr)
        {
            if(!SDL_SetWindowParent(_window, owner->_window))
            {
                SDL_Log("Unable to set a parent window: %s", SDL_GetError());
            }
        }
        else
        {
            SDL_SetWindowParent(_window, nullptr);
        }
    }

    Window* Window::GetOwner() const
    {
        return _owner;
    }

    void Window::SetContent(Ref<UIElement> value)
    {
        if(_content)
        {
            _content->SetParent(nullptr);
            _content->SetVisualRoot(nullptr);
            _content->OnDetachedFromElementsTree();
        }

        SetProperty(ContentProperty, _content, std::move(value));

        if(_content)
        {
            _content->SetParent(this);
            _content->SetVisualRoot(this);
            _content->OnAttachedToElementsTree();
        }
    }

    Ref<UIElement> Window::GetContent() const 
    {
        return _content; 
    }    

    void Window::Show()
    {
        App->AddActiveWindow(this);
        SDL_ShowWindow(_window);
    }

    void Window::ShowModal(Window& owner)
    {
        _isModal = true;

        SetOwner(&owner);

        if(!SDL_SetWindowModal(_window, true))
        {
            SDL_Log("Unable to set a modal window: %s", SDL_GetError());
        }

        SetPosition(DefaultPosition);
        Show();
    }

    void Window::Hide()
    {
        _isClosing = false;
        SDL_HideWindow(_window);
    }

    void Window::Close()
    {
        _isClosing = true;
        SDL_HideWindow(_window);
    }

    void Window::Activate()
    {
        SDL_RaiseWindow(_window);
    }

    bool Window::IsVisible() const
    {
        return !(SDL_GetWindowFlags(_window) & (SDL_WINDOW_HIDDEN | SDL_WINDOW_MINIMIZED));
    }

    bool Window::IsModal() const
    {
        return _isModal;
    }

    void Window::Render(RenderContext context)
    {
        std::visit(WindowBackgroundRenderer(context), GetBackground());

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
        if(!IsVisible())
        {
            return;
        }

        if(!(_content && _content->IsMouseOver()))
        {
            Cursor::Set(cursor);
        }
    }

    void Window::OnWindowStateChanged(WindowStateChangedEventArgs& e)
    {
        WindowStateChanged(*this, e);
    }

    void Window::OnPositionChanged(WindowPositionChangedEventArgs& e)
    {
        PositionChanged(*this, e);
    }

    void Window::OnWindowSizeChanged(WindowSizeChangedEventArgs& e)
    {
        if(_content)
        {
            _content->InvalidateMeasure();
        }

        SizeChanged(*this, e);
    }

    void Window::OnMouseMove(MouseEventArgs& e)
    {
        if(_content && _content->IsVisible())
        {
            auto& content = _content.GetValue();
            bool wasMouseOver = content._isMouseOver;
            bool isMouseOver = LayoutHelper::IsPointInRect(e.X, e.Y, content._bounds);

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

    void Window::OnActivated()
    {
        if(_content && _content->IsMouseOver())
        {
            Cursor::Set(_content->GetCursor());
        }
        else
        {
            Cursor::Set(GetCursor());
        }
    }

    void Window::OnDeactivated()
    {
        if(_isClosing)
        {
            App->RemoveActiveWindow(this);

            if(_isModal)
            {
                SDL_SetWindowModal(_window, false);
                SetOwner(nullptr);
            }
        }
    }

    void Window::RenderCore()
    {
        if(IsVisible() || IsRenderableWhenMinimized)
        {
            if(NeedsRendering())
            {
                Render(_renderContext);
                SDL_RenderPresent(_renderer);
            }
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