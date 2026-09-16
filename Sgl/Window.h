#pragma once

#include <optional>
#include <coroutine>

#include "Base/Event.h"
#include "Render/Surface.h"
#include "Input/TextEventArgs.h"
#include "UIElements/UIElement.h"
#include "Input/InputManager.h"

namespace Sgl
{
	//! @brief Specifies the display mode of a window
	enum class WindowDisplayMode
	{
		Window,           // Standard window with borders and title bar
		BorderlessWindow, // Window without borders, typically spanning the screen but not exclusive
		Fullscreen        // Exclusive fullscreen mode
	};

	//! @brief Represents the current state of a window
	enum class WindowState
	{
		Normal,     // The window is in its standard restored state
		Minimized,  // The window is minimized (e.g., to the taskbar)
		Maximized   // The window is maximized to fill the screen (or work area)
	};

	//! @brief Event arguments provided when the window's state changes (e.g., minimized, maximized)
	struct WindowStateChangedEventArgs
	{
		WindowState State;
	};

	//! @brief Event arguments provided when the window's position changes on the screen
	struct WindowPositionChangedEventArgs
	{
		int X;
		int Y;
	};

	//! @brief Event arguments provided when the window's client size changes
	struct WindowSizeChangedEventArgs
	{
		int Width;
		int Height;
	};

	//! @brief Event arguments used to allow cancellation of an operation (e.g., closing a window)
	struct CancelEventArgs
	{
		bool Cancel;
	};

	//! @brief Represents the result of a dialog or modal interaction
	enum class DialogResult
	{
		None,    // No result has been set (default or pending state)
		Accept,  // The user accepted the dialog (e.g., clicked OK)
		Cancel   // The user canceled the dialog (e.g., clicked Cancel)
	};

	//! @brief The Window class provides a high-level interface for creating and managing window, handling events, and rendering graphics.
	//! It encapsulates SDL_Window and SDL_Renderer objects and provides convenient methods for common window operations.
	class Window : public Renderable, public IVisualRoot
	{
	public:
		struct Content;
		class ModalAwaiter;
		using WindowEventHandler = EventHandler<Window>;
		using WindowStateEventHandler = EventHandler<Window, WindowStateChangedEventArgs>;
		using WindowPositionChangedEventHandler = EventHandler<Window, WindowPositionChangedEventArgs>;
		using WindowSizeChangedEventHandler = EventHandler<Window, WindowSizeChangedEventArgs>;
		using CancelEventHandler = EventHandler<Window, CancelEventArgs&>;
	public:
		Window();

		//! @brief Event triggered when the window's state changes (minimized, maximized, restored)
		Event<WindowStateEventHandler> WindowStateChanged;

		//! @brief Event triggered when the window's position changes
		Event<WindowPositionChangedEventHandler> PositionChanged;

		//! @brief Event triggered when the window's size changes
		Event<WindowSizeChangedEventHandler> SizeChanged;

		//! @brief Event triggered when the window is about to close
		Event<CancelEventHandler> Closing;

		//! @brief Event triggered after the window has been closed
		Event<WindowEventHandler> Closed;

		//! @brief The result returned by this dialog when it closes
		DialogResult DialogResult = DialogResult::None;

		//! @brief Gets the underlying SDL window handle
		//! @return Pointer to the SDL_Window
		SDL_Window* GetSDLWindow() const noexcept;

		//! @brief Gets the underlying SDL renderer handle
		//! @return Pointer to the SDL_Renderer
		SDL_Renderer* GetRenderer() const override;

		//! @brief Gets window id
		//! @return Window unique id
		SDL_WindowID GetId() const noexcept;

		//! @brief Sets the window width
		//! @param value The new width in pixels
		void SetWidth(uint32_t value) noexcept;

		//! @brief Gets the window width
		//! @return The current width in pixels
		uint32_t GetWidth() const noexcept;

		//! @brief Sets the window height
		//! @param value The new height in pixels
		void SetHeight(uint32_t value) noexcept;

		//! @brief Gets the window height
		//! @return The current height in pixels
		uint32_t GetHeight() const noexcept;

		//! @brief Sets the window width and height
		//! @param size The new width and height in pixels
		void SetSize(Size size) noexcept;

		//! @brief Gets the window size
		//! @return The current width and height in pixels
		Size GetSize() const noexcept;

		//! @brief Sets the window title
		//! @param value The new window title
		void SetTitle(std::string_view value) noexcept;

		//! @brief Gets the window title
		//! @return String view of the current window title
		std::string_view GetTitle() const noexcept;

		//! @brief Sets the maximum size of the window
		//! @param size The maximum size
		void SetMaxSize(Size size) noexcept;

		//! @brief Gets the maximum size of the window
		//! @return The current maximum size
		Size GetMaxSize() const noexcept;

		//! @brief Sets the minimum size of the window
		//! @param size The minimum size
		void SetMinSize(Size size) noexcept;

		//! @brief Gets the minimum size of the window
		//! @return The current minimum size
		Size GetMinSize() const noexcept;

		//! @brief Sets the window position
		//! @param value The new position
		void SetPosition(Point value) noexcept;

		//! @brief Gets the window position
		//! @return The current position
		Point GetPosition() const noexcept;

		//! @brief Sets the window display mode (Window, Borderless, Fullscreen)
		//! @param displayMode The display mode to set
		void SetDisplayMode(WindowDisplayMode displayMode);

		//! @brief Gets the current display mode
		//! @return The current display mode
		WindowDisplayMode GetDisplayMode() const noexcept;

		//! @brief Sets the window state (Normal, Minimized, Maximized)
		//! @param state The new window state
		void SetWindowState(WindowState state) noexcept;

		//! @brief Gets the current window state
		//! @return The current window state
		WindowState GetWindowState() const noexcept;

		//! @brief Sets the window icon
		//! @param icon An icon surface
		void SetIcon(Surface icon);

		//! @brief Gets the current window icon
		//! @return Path to the icon
		Surface GetIcon() const;

		//! @brief Sets whether the window is resizable
		//! @param value True to make resizable, false otherwise
		void SetResizable(bool value = true) noexcept;

		//! @brief Checks if the window is resizable
		//! @return True if resizable, false otherwise
		bool IsResizable() const;

		//! @brief Sets whether the window should always appear on top of other windows
		//! @param value True to make the window stay on top; false to restore normal z-order. Defaults to true.
		void SetAlwayOnTop(bool value = true);

		//! @brief Checks whether the window is set to always stay on top
		//! @return True if the window is always on top; otherwise, false
		bool IsAlwayOnTop() const;

		//! @brief Sets the owner of this window. Owned windows are typically modal.
		//! @param owner Pointer to the owner window, or nullptr to remove ownership
		void SetOwner(Window* owner);

		//! @brief Gets the current owner of this window
		//! @return Pointer to the owner window, or nullptr if no owner is set
		Window* GetOwner() const;

		//! @brief Gets a list of windows that are owned by this window
		//! @return A const reference to the list of owned windows
		const std::list<Ref<Window>>& GetOwnedWindows() const;

		//! @brief Sets the root UI element displayed in the window's client area
		//! @param value The UI element to use as content
		//! @param source The source of the value. Defaults to ValueSource::Local.
		void SetContent(const Ref<UIElement>& value, ValueSource source = ValueSource::Local);

		//! @brief Gets the root UI element currently displayed in the window
		//! @return A reference to the current content element
		const Ref<UIElement>& GetContent() const noexcept { return _content; }

		//! @brief Returns the topmost active element at the specified point
		//! @param point The point on the element
		//! @return Reference to the topmost active element
		Ref<UIElement> HitTest(FPoint point) const;

		//! @brief Applies style for the window and its content
		void ApplyStyle() final;

		//! @brief Marks the window's visual content as invalid, requesting a re-render on the next frame
		void MarkDirty() final;

		//! @brief Provides access to the texture factory instance
		//! @return A reference to the active ITextureFactory implementation
		ITextureFactory& GetTextureFactory() final;

		//! @brief Gets focus manager
		//! @return Reference to focus manager
		FocusManager& GetFocusManager();

		//! @brief Checks whether the window needs to be rendered due to pending changes
		//! @return True if rendering is required; otherwise, false
		bool NeedsRendering() const noexcept;

		//! @brief Shows the window
		void Show();

		//! @brief Shows this window as a modal dialog centered over the specified owner window
		//! @param owner The parent window that owns this modal dialog
		//! @return ModalAwaiter object that can be co_awaited
		ModalAwaiter ShowModal(Window& owner);

		//! @brief Hides the window
		void Hide();

		//! @brief Closes the window
		void Close();

		//! @brief Window gains focus
		void Activate();

		//! @brief Checks if the window has focus
		//! @return True if it has focus; otherwise, false
		bool IsActivated() const noexcept;

		//! @brief Checks if the window is visible
		//! @return True if visible; otherwise, false
		bool IsVisible() const;

		//! @brief Checks whether the window has been closed
		//! @return True if the window is closed; otherwise, false
		bool IsClosed() const noexcept;

		//! @brief Renders the window's content using the provided rendering context
		//! @param context The render context used to draw UI elements
		void Render(RenderContext context) override;

		//! @brief Processes window-specific logic
		virtual void Process();

		static inline StyleableProperty ContentProperty { &SetContent, &GetContent };
	protected:
		~Window();

		void OnCursorChanged(Cursor cursor) final;
		void OnBackgroundChanged(const Brush& background) final;
		void OnDataContextChanged(const Ref<ObservableObject>& dataContext) final;
		void OnAttachedToLogicalTree() override;
		void OnDetachedFromLogicalTree() override;

		virtual void OnWindowStateChanged(WindowStateChangedEventArgs e);
		virtual void OnPositionChanged(WindowPositionChangedEventArgs e);
		virtual void OnWindowSizeChanged(WindowSizeChangedEventArgs e);
		virtual void OnKeyUp(KeyEventArgs e);
		virtual void OnKeyDown(KeyEventArgs e);
		virtual void OnMouseMove(MouseMoveEventArgs e);
		virtual void OnMouseDown(MouseClickEventArgs& e);
		virtual void OnMouseUp(MouseClickEventArgs& e);
		virtual void OnMouseWheelChanged(MouseWheelEventArgs& e);
		virtual void OnTextInput(TextInputEventArgs& e) {}
		virtual void OnTextEditing(TextEditingEventArgs& e) {}
		virtual void OnMouseEnter() {}
		virtual void OnMouseLeave() {}
		virtual void OnActivated();
		virtual void OnDeactivated();
		virtual void OnShown();
		virtual void OnClosing(CancelEventArgs& e);
		virtual void OnClosed();
	private:
		void RenderCore();
		void DestroyRenderer();
	private:
		SDL_WindowID _id = 0;
		SDL_Window* _sdlWindow;
		SDL_Renderer* _renderer;
		InputManager _inputManager;
		ITextureFactory* _textureFactory;
		Ref<UIElement> _content;
		bool _isModal = false;
		bool _isClosing = false;
		bool _isClosed = true;
		bool _isActivated = false;
		bool _isRenderValid = false;
		Window* _owner = nullptr;
		std::list<Ref<Window>> _ownedWindows;
		Surface _icon;
		RenderFragment _backgroundFragment;
		ValueSource _contentSource {};

		friend class UIElement;
		friend class Application;
	};

	struct Window::Content
	{
		Styleable& operator()(Styleable& element) const
		{
			return static_cast<Window&>(element).GetContent().GetValue();
		}
	};

	class Window::ModalAwaiter
	{
	public:
		explicit ModalAwaiter(Window& window);
		~ModalAwaiter();

		bool await_ready();
		void await_suspend(std::coroutine_handle<> handle);
		void await_resume() {}
	private:
		Window& _window;
		std::coroutine_handle<> _handle;
		bool _hasSuspended = false;
	};
}