#include "Window.h"

#include "Application.h"
#include "Render/BackgroundRenderer.h"
#include "Layout/LayoutHelper.h"
#include "Base/Exceptions.h"
#include "Base/Logger.h"
#include "Input/UserEvents.h"

namespace Sgl
{
    static constexpr auto DefaultTitle = "Window";
    static constexpr auto DefaultWidth = 1280;
    static constexpr auto DefaultHeight = 720;
    static constexpr auto DefaultPosition = Point(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    
    Window::Window():
        _sdlWindow(SDL_CreateWindow(DefaultTitle, DefaultWidth, DefaultHeight, SDL_WINDOW_HIDDEN)),
        _renderer(SDL_CreateRenderer(_sdlWindow, nullptr)),
        _renderContext(_renderer)
    {
        if(_sdlWindow == nullptr)
        {
            throw Exception("Unable to create a window: {}", SDL_GetError());
        }

        if(_renderer == nullptr)
        {
            throw Exception("Unable to create a renderer: {}", SDL_GetError());
        }

        if(App.Current() == nullptr)
        {
            throw Exception("Cannot create a window without an application");
        }

        _id = SDL_GetWindowID(_sdlWindow);
        App->AddWindow(*this);
        SetVisualRoot(this);
        SetBackground(Colors::White);
    }

    Window::~Window()
    {
        if(_content)
        {
            _content->OnDetachedFromLogicalTree();
            _content = nullptr;
        }

        Close();

        if(App.Current() == nullptr)
        {
            throw Exception("Appication does not exist");
        }

        App->RemoveWindow(*this);
        SDL_DestroyRenderer(_renderer);
        SDL_DestroyWindow(_sdlWindow);
    }

    SDL_Window* Window::GetSDLWindow() const noexcept
    {
        return _sdlWindow;
    }

    SDL_Renderer* Window::GetRenderer() const
    {
        return _renderer;
    }

    SDL_WindowID Window::GetId() const noexcept
    {
        return _id;
    }

    void Window::SetWidth(size_t value) noexcept
    {
        SDL_SetWindowSize(_sdlWindow, value, GetHeight());
    }

    size_t Window::GetWidth() const noexcept
    {
        int width = 0;
        SDL_GetWindowSize(_sdlWindow, &width, nullptr);
        return width;
    }

    void Window::SetHeight(size_t value) noexcept
    {
        SDL_SetWindowSize(_sdlWindow, GetWidth(), value);
    }

    size_t Window::GetHeight() const noexcept
    {
        int height = 0;
        SDL_GetWindowSize(_sdlWindow, nullptr, &height);
        return height;
    }

    void Window::SetSize(Size size) noexcept
    {
        SDL_SetWindowSize(_sdlWindow, size.Width, size.Height);
    }

    Size Window::GetSize() const noexcept
    {
        int width = 0, height = 0;
        SDL_GetWindowSize(_sdlWindow, &width, &height);
        return Size(width, height);
    }

    void Window::SetMaxSize(Size size) noexcept
    {
        SDL_SetWindowMaximumSize(_sdlWindow, size.Width, size.Height);
    }

    Size Window::GetMaxSize() const noexcept
    {
        int width = 0, height = 0;
        SDL_GetWindowMaximumSize(_sdlWindow, &width, &height);
        return Size(width, height);
    }

    void Window::SetMinSize(Size size) noexcept
    {        
        SDL_SetWindowMinimumSize(_sdlWindow, size.Width, size.Height);
    }

    Size Window::GetMinSize() const noexcept
    {
        int width = 0, height = 0;
        SDL_GetWindowMinimumSize(_sdlWindow, &width, &height);
        return Size(width, height);
    }

    void Window::SetTitle(std::string_view value) noexcept
    {
        SDL_SetWindowTitle(_sdlWindow, value.data());
    }

    std::string_view Window::GetTitle() const noexcept
    {
        return SDL_GetWindowTitle(_sdlWindow);
    }

    void Window::SetPosition(Point value) noexcept
    {
        SDL_SetWindowPosition(_sdlWindow, value.x, value.y);
    }

    Point Window::GetPosition() const noexcept
    {
        Point position = {};
        SDL_GetWindowPosition(_sdlWindow, &position.x, &position.y);
        return position;
    }

    void Window::SetDisplayMode(WindowDisplayMode displayMode)
    {
        switch(displayMode)
        {
            case WindowDisplayMode::Window: 
                SDL_SetWindowFullscreen(_sdlWindow, false);
                SDL_SetWindowBordered(_sdlWindow, true);
                break;

            case WindowDisplayMode::BorderlessWindow:
                SDL_SetWindowFullscreen(_sdlWindow, false);
                SDL_SetWindowBordered(_sdlWindow, false);
                break;

            case WindowDisplayMode::Fullscreen:
                SDL_SetWindowFullscreen(_sdlWindow, true);
                break;
        }
    }

    WindowDisplayMode Window::GetDisplayMode() const noexcept
    {
        auto flags = SDL_GetWindowFlags(_sdlWindow);
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
                SDL_RestoreWindow(_sdlWindow);
                break;

            case Sgl::WindowState::Minimized:
                SDL_MinimizeWindow(_sdlWindow);
                break;

            case Sgl::WindowState::Maximized:
                SDL_MaximizeWindow(_sdlWindow);
                break;
        }
    }

    WindowState Window::GetWindowState() const noexcept
    {
        auto flags = SDL_GetWindowFlags(_sdlWindow);
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

        if(!SDL_SetWindowIcon(_sdlWindow, _icon.GetSDLSurface()))
        {
            Logger::LogWarning("Unable to set a window icon: {}", SDL_GetError());
        }
    }

    const std::string& Window::GetIcon() const
    {
        return _iconSource;
    }

    void Window::SetResizable(bool value) noexcept
    {
        SDL_SetWindowResizable(_sdlWindow, value);
    }

    bool Window::IsResizable() const
    {
        return SDL_GetWindowFlags(_sdlWindow) & SDL_WINDOW_RESIZABLE;
    }

    void Window::SetAlwayOnTop(bool value)
    {
        SDL_SetWindowAlwaysOnTop(_sdlWindow, value);
    }

    bool Window::IsAlwayOnTop() const
    {
        return SDL_GetWindowFlags(_sdlWindow) & SDL_WINDOW_ALWAYS_ON_TOP;
    }

    void Window::SetOwner(Window* owner)
    {
        if(_owner == owner)
        {
            return;
        }

        if(_owner)
        {
            std::erase(_owner->_ownedWindows, this);
        }

        _owner = owner;

        if(owner != nullptr)
        {
            if(SDL_SetWindowParent(_sdlWindow, owner->_sdlWindow))
            {
                owner->_ownedWindows.push_back(this);
            }
            else
            {
                Logger::LogWarning("Unable to set an owner for a window: {}", SDL_GetError());
            }
        }
        else
        {
            SDL_SetWindowParent(_sdlWindow, nullptr);
        }
    }

    Window* Window::GetOwner() const
    {
        return _owner;
    }

    const std::list<Window*>& Window::GetOwnedWindows() const
    {
        return _ownedWindows;
    }

    void Window::SetContent(Ref<UIElement> value)
    {
        if(_content)
        {
            if(IsAttachedToLogicalTree())
            {
                _content->OnDetachedFromLogicalTree();
            }

            _content->SetParent(nullptr);
        }

        SetProperty(ContentProperty, _content, std::move(value));

        if(_content)
        {
            _content->SetParent(this);

            if(IsAttachedToLogicalTree())
            {
                _content->OnAttachedToLogicalTree();
            }
        }
    }

    Ref<UIElement> Window::GetContent() const 
    {
        return _content; 
    }

    void Window::InvalidateRender()
    {
        _isRenderValid = false;
    }

    void Window::Show()
    {
        if(IsVisible())
        {
            return;
        }

        App->AttachWindow(*this);
        SDL_ShowWindow(_sdlWindow);
    }

    ModalWindowAwaitable Window::ShowModal(Window& owner)
    {
        if(IsVisible())
        {
            return ModalWindowAwaitable(*this);
        }

        _isModal = true;

        SetOwner(&owner);

        if(!SDL_SetWindowModal(_sdlWindow, true))
        {
            Logger::LogWarning("Unable to set a modal window: {}", SDL_GetError());
        }

        SetPosition(DefaultPosition);
        Show();

        return ModalWindowAwaitable(*this);
    }

    void Window::Hide()
    {
        SDL_HideWindow(_sdlWindow);
    }

    void Window::Close()
    {
        SDL_Event e;
        e.window =
        {
            .type = SDL_EVENT_WINDOW_CLOSE_REQUESTED,
            .windowID = _id
        };
        SDL_PushEvent(&e);
    }

    void Window::Activate()
    {
        SDL_RaiseWindow(_sdlWindow);
    }

    bool Window::IsVisible() const
    {
        return !(SDL_GetWindowFlags(_sdlWindow) & SDL_WINDOW_HIDDEN);
    }

    void Window::SetStylingRoot(IStyleHost* value)
    {
        StyleableElement::SetStylingRoot(value);

        if(_content)
        {
            _content->SetStylingRoot(value);
        }
    }

    void Window::Render(RenderContext context)
    {
        std::visit(WindowBackgroundRenderer(context), GetBackground());

        if(_content && _content->IsVisible())
        {
            _content->Render(context);
        }        

        _isRenderValid = true;
    }

    void Window::ApplyStyle()
    {
        StyleableElement::ApplyStyle();

        if(_content)
        {
            _content->ApplyStyle();
        }
    }

    void Window::Process()
    {
        if(_content)
        {
            if(_content->NeedsMeasure())
            {
                auto [width, height] = GetSize();
                _content->Measure(FSize(width, height));
            }

            if(_content->NeedsArrange())
            {
                auto [width, height] = GetSize();
                _content->Arrange(FRect(0, 0, width, height));
            }
        }
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

    void Window::OnAttachedToLogicalTree()
    {
        StyleableElement::OnAttachedToLogicalTree();

        if(_content)
        {
            _content->OnAttachedToLogicalTree();
        }
    }

    void Window::OnDetachedFromLogicalTree()
    {
        StyleableElement::OnDetachedFromLogicalTree();

        if(_content)
        {
            _content->OnDetachedFromLogicalTree();
        }
    }

    void Window::OnWindowStateChanged(WindowStateChangedEventArgs e)
    {
        WindowStateChanged(*this, e);
    }

    void Window::OnPositionChanged(WindowPositionChangedEventArgs e)
    {
        PositionChanged(*this, e);
    }

    void Window::OnWindowSizeChanged(WindowSizeChangedEventArgs e)
    {
        if(_content)
        {
            _content->InvalidateMeasure();
        }

        SizeChanged(*this, e);
    }

    void Window::OnMouseMove(MouseMoveEventArgs e)
    {
        if(_content)
        {
            bool visible = _content->IsVisible();
            bool wasMouseOver = _content->_isMouseOver;
            bool isMouseOver = visible && LayoutHelper::IsPointInRect(e.X, e.Y, _content->_bounds);

            if(isMouseOver)
            {
                if(!wasMouseOver)
                {
                    _content->OnMouseEnter(e);
                }

                _content->OnMouseMove(e);
            }
            else if(wasMouseOver)
            {
                _content->OnMouseLeave(e);
                Cursor::Set(GetCursor());
            }
        }
    }

    void Window::OnMouseDown(MouseButtonEventArgs e)
    {
        if(_content && _content->IsMouseOver() && _content->IsVisible())
        {
            _content->OnMouseDown(e);
        }
    }

    void Window::OnMouseUp(MouseButtonEventArgs e)
    {
        if(_content && _content->IsMouseOver() && _content->IsVisible())
        {
            _content->OnMouseUp(e);
        }
    }

    void Window::OnActivated()
    {
        _isActivated = true;

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
        _isActivated = false;
    }

    void Window::OnShown()
    {
        _isClosed = false;
    }

    void Window::OnClosing(CancelEventArgs& e)
    {   
        Closing(*this, e);

        if(e.Cancel)
        {
            return;
        }

        _isClosing = true;
        App->DetachWindow(*this);              
    }

    void Window::OnClosed()
    {
        _isClosing = false;
        _isClosed = true;

        if(_isModal)
        {
            SDL_SetWindowModal(_sdlWindow, false);
            SetOwner(nullptr);
            _isModal = false;
        }

        Closed(*this);
    }

    void Window::RenderCore()
    {
        if(NeedsRendering() && !IsRenderableWhenMinimized)
        {
            Render(_renderContext);
            SDL_RenderPresent(_renderer);
        }
    }    
}