#pragma once

#include <optional>
#include <coroutine>
#include "Base/Event.h"
#include "Base/Logging.h"
#include "Base/Threading/ModalWindowAwaitable.h"
#include "Render/Surface.h"
#include "Input/TextEventArgs.h"
#include "UIElement/UIElement.h"

namespace Sgl
{
	/// <summary>
	/// Specifies the display mode of a window.
	/// </summary>
	enum class WindowDisplayMode
	{
		Window,           // Standard window with borders and title bar.
		BorderlessWindow, // Window without borders, typically spanning the screen but not exclusive.
		Fullscreen        // Exclusive fullscreen mode.
	};

	/// <summary>
	/// Represents the current state of a window.
	/// </summary>
	enum class WindowState
	{
		Normal,     // The window is in its standard restored state.
		Minimized,  // The window is minimized (e.g., to the taskbar).
		Maximized   // The window is maximized to fill the screen (or work area).
	};

	/// <summary>
	/// Event arguments provided when the window's state changes (e.g., minimized, maximized).
	/// </summary>
	struct WindowStateChangedEventArgs
	{
		WindowState State;
	};

	/// <summary>
	/// Event arguments provided when the window's position changes on the screen.
	/// </summary>
	struct WindowPositionChangedEventArgs
	{
		int X;
		int Y;
	};

	/// <summary>
	/// Event arguments provided when the window's client size changes.
	/// </summary>
	struct WindowSizeChangedEventArgs
	{
		int Width;
		int Height;
	};

	/// <summary>
	/// Event arguments used to allow cancellation of an operation (e.g., closing a window).
	/// </summary>
	struct CancelEventArgs
	{
		bool Cancel;
	};

	/// <summary>
	/// Represents the result of a dialog or modal interaction.
	/// </summary>
	enum class DialogResult
	{
		None,    // No result has been set (default or pending state).
		Accept,  // The user accepted the dialog (e.g., clicked OK).
		Cancel   // The user canceled the dialog (e.g., clicked Cancel).
	};

	/// <summary>
	/// The Window class provides a high-level interface for creating and managing window,
	/// handling events, and rendering graphics. It encapsulates SDL_Window and SDL_Renderer
	/// objects and provides convenient methods for common window operations.
	/// </summary>
	class Window : public Renderable, public IVisualRoot
	{
	private:
		using WindowEventHandler = EventHandler<Window>;
		using WindowStateEventHandler = EventHandler<Window, WindowStateChangedEventArgs>;
		using WindowPositionChangedEventHandler = EventHandler<Window, WindowPositionChangedEventArgs>;
		using WindowSizeChangedEventHandler = EventHandler<Window, WindowSizeChangedEventArgs>;
		using CancelEventHandler = EventHandler<Window, CancelEventArgs&>;
	public:
		Window();
		Window(const Window&) = delete;
		Window(Window&&) = delete;
		~Window();

		/// <summary>
		/// Event triggered when the window's state changes (minimized, maximized, restored)
		/// </summary>
		Event<WindowStateEventHandler> WindowStateChanged;

		/// <summary>
		/// Event triggered when the window's position changes
		/// </summary>
		Event<WindowPositionChangedEventHandler> PositionChanged;

		/// <summary>
		/// Event triggered when the window's size changes
		/// </summary>
		Event<WindowSizeChangedEventHandler> SizeChanged;

		/// <summary>
		/// Event triggered when the window is about to close.
		/// </summary>
		Event<CancelEventHandler> Closing;

		/// <summary>
		/// Event triggered after the window has been closed.
		/// </summary>
		Event<WindowEventHandler> Closed;

		/// <summary>
		/// Determines if the window should be rendered when minimized
		/// </summary>
		bool IsRenderableWhenMinimized = false;

		/// <summary>
		/// The result returned by this dialog when it closes.
		/// </summary>
		DialogResult DialogResult = DialogResult::None;

		/// <summary>
		/// Provides a convenient way to load textures (e.g., icons, images) using the window's renderer.
		/// </summary>
		TextureLoader ImageLoader;

		/// <summary>
		/// Gets the underlying SDL window handle
		/// </summary>
		/// <returns>Pointer to the SDL_Window</returns>
		SDL_Window* GetSDLWindow() const noexcept;

		/// <summary>
		/// Gets the underlying SDL renderer handle
		/// </summary>
		/// <returns>Pointer to the SDL_Renderer</returns>
		SDL_Renderer* GetRenderer() const override;

		/// <summary>
		/// Gets window id
		/// </summary>
		/// <returns>Window unique id</returns>
		SDL_WindowID GetId() const noexcept;

		/// <summary>
		/// Sets the window width
		/// </summary>
		/// <param name="value">- the new width in pixels</param>
		void SetWidth(uint32_t value) noexcept;

		/// <summary>
		/// Gets the window width
		/// </summary>
		/// <returns>The current width in pixels</returns>
		uint32_t GetWidth() const noexcept;

		/// <summary>
		/// Sets the window height
		/// </summary>
		/// <param name="value">- the new height in pixels</param>
		void SetHeight(uint32_t value) noexcept;

		/// <summary>
		/// Gets the window height
		/// </summary>
		/// <returns>The current height in pixels</returns>
		uint32_t GetHeight() const noexcept;

		/// <summary>
		/// Set the width width and height
		/// </summary>
		/// <param name="size">- the new width and height in pixels</param>
		void SetSize(Size size) noexcept;

		/// <summary>
		/// Gets the window size
		/// </summary>
		/// <returns>The current width and height in pixels</returns>
		Size GetSize() const noexcept;

		/// <summary>
		/// Sets the window title
		/// </summary>
		/// <param name="value">- the new window title</param>
		void SetTitle(std::string_view value) noexcept;

		/// <summary>
		/// Gets the window title
		/// </summary>
		/// <returns>String view of the current window title</returns>
		std::string_view GetTitle() const noexcept;

		/// <summary>
		/// Sets the maximum size of the window
		/// </summary>
		/// <param name="size">- the maximum size</param>
		void SetMaxSize(Size size) noexcept;

		/// <summary>
		/// Gets the maximum size of the window
		/// </summary>
		/// <returns>The current maximum size</returns>
		Size GetMaxSize() const noexcept;

		/// <summary>
		/// Sets the minimum size of the window
		/// </summary>
		/// <param name="size">- the minimum size</param>
		void SetMinSize(Size size) noexcept;

		/// <summary>
		/// Gets the minimum size of the window
		/// </summary>
		/// <returns>The current minimum size</returns>
		Size GetMinSize() const noexcept;

		/// <summary>
		/// Sets the window position
		/// </summary>
		/// <param name="value">- the new position</param>
		void SetPosition(Point value) noexcept;

		/// <summary>
		/// Gets the window position
		/// </summary>
		/// <returns>The current position</returns>
		Point GetPosition() const noexcept;

		/// <summary>
		/// Sets the window display mode (Window, Borderless, Fullscreen)
		/// </summary>
		/// <param name="displayMode">- the display mode to set</param>
		void SetDisplayMode(WindowDisplayMode displayMode);

		/// <summary>
		/// Gets the current display mode
		/// </summary>
		/// <returns>The current display mode</returns>
		WindowDisplayMode GetDisplayMode() const noexcept;

		/// <summary>
		/// Sets the window state (Normal, Minimized, Maximized)
		/// </summary>
		/// <param name="state">- the new window state</param>
		void SetWindowState(WindowState state) noexcept;

		/// <summary>
		/// Gets the current window state
		/// </summary>
		/// <returns>The current window state</returns>
		WindowState GetWindowState() const noexcept;

		/// <summary>
		/// Sets the window icon
		/// </summary>
		/// <param name="icon">- an icon surface</param>
		void SetIcon(const Surface& icon);

		/// <summary>
		/// Gets the current window icon
		/// </summary>
		/// <returns>Path to the icon</returns>
		const Surface& GetIcon() const;

		/// <summary>
		/// Sets whether the window is resizable
		/// </summary>
		/// <param name="value">- true to make resizable, false otherwise</param>
		void SetResizable(bool value = true) noexcept;

		/// <summary>
		/// Checks if the window is resizable
		/// </summary>
		/// <returns>True if resizable, false otherwise</returns>
		bool IsResizable() const;

		/// <summary>
		/// Sets whether the window should always appear on top of other windows.
		/// </summary>
		/// <param name="value"> - true to make the window stay on top; false to restore normal z-order. Defaults to true.</param>
		void SetAlwayOnTop(bool value = true);

		/// <summary>
		/// Checks whether the window is set to always stay on top.
		/// </summary>
		/// <returns>True if the window is always on top; otherwise, false.</returns>
		bool IsAlwayOnTop() const;

		/// <summary>
		/// Sets the owner of this window. Owned windows are typically modal.
		/// </summary>
		/// <param name="owner"> - pointer to the owner window, or nullptr to remove ownership.</param>
		void SetOwner(Window* owner);

		/// <summary>
		/// Gets the current owner of this window.
		/// </summary>
		/// <returns>Pointer to the owner window, or nullptr if no owner is set.</returns>
		Window* GetOwner() const;

		/// <summary>
		/// Gets a list of windows that are owned by this window.
		/// </summary>
		/// <returns>A const reference to the list of owned windows.</returns>
		const std::list<Window*>& GetOwnedWindows() const;

		/// <summary>
		/// Sets the root UI element displayed in the window's client area.
		/// </summary>
		/// <param name="value"> - the UI element to use as content.</param>
		/// <param name="source"> - the source of the value. Defaults to ValueSource::Local.</param>
		void SetContent(const Ref<UIElement>& value, ValueSource source = ValueSource::Local);

		/// <summary>
		/// Gets the root UI element currently displayed in the window.
		/// </summary>
		/// <returns>A reference to the current content element.</returns>
		const Ref<UIElement>& GetContent() const { return _content; }

		/// <summary>
		/// Marks the window's visual content as invalid, requesting a re-render on the next frame.
		/// </summary>
		void InvalidateRender() final;

		/// <summary>
		/// Checks whether the window needs to be rendered due to pending changes.
		/// </summary>
		/// <returns>True if rendering is required; otherwise, false.</returns>
		bool NeedsRendering() const final { return !_isRenderValid; }

		/// <summary>
		/// Shows the window
		/// </summary>
		void Show();

		/// <summary>
		/// Shows this window as a modal dialog centered over the specified owner window.
		/// </summary>
		/// <param name="owner"> - the parent window that owns this modal dialog</param>
		/// <returns>WindowModalAwaitable - object that can be co_awaited</returns>
		ModalWindowAwaitable ShowModal(Window& owner);

		/// <summary>
		/// Hides the window
		/// </summary>
		void Hide();

		/// <summary>
		/// Close the window
		/// </summary>
		void Close();

		/// <summary>
		/// Window gain a focus
		/// </summary>
		void Activate();

		/// <summary>
		/// Checks if the window has a focus
		/// </summary>
		/// <returns>True if has focus, false otherwise</returns>
		bool IsActivated() const { return _isActivated; }

		/// <summary>
		/// Checks if the window is visible
		/// </summary>
		/// <returns>- true if visible, false otherwise</returns>
		bool IsVisible() const;

		/// <summary>
		/// Checks whether the window has been closed.
		/// </summary>
		/// <returns>True if the window is closed; otherwise, false.</returns>
		bool IsClosed() const { return _isClosed; }

		/// <summary>
		/// Sets the styling root used to resolve styles and resources for this window.
		/// </summary>
		/// <param name="value"> - pointer to the style host.</param>
		void SetStylingRoot(IStyleHost* value) final;

		/// <summary>
		/// Renders the window's content using the provided rendering context.
		/// </summary>
		/// <param name="context"> - the render context used to draw UI elements.</param>
		void Render(RenderContext context) override;

		/// <summary>
		/// Applies the current style rules to this window and its visual tree.
		/// </summary>
		void ApplyStyle() override;

		/// <summary>
		/// Processes window-specific logic.
		/// </summary>
		virtual void Process();

		static inline StyleableProperty ContentProperty { &SetContent, &GetContent };
	protected:
		void OnCursorChanged(const Cursor& cursor) override;
		void OnDataContextChanged(const Ref<INotifyPropertyChanged>& dataContext) override;
		void OnAttachedToLogicalTree() override;
		void OnDetachedFromLogicalTree() override;
		virtual void OnWindowStateChanged(WindowStateChangedEventArgs e);
		virtual void OnPositionChanged(WindowPositionChangedEventArgs e);
		virtual void OnWindowSizeChanged(WindowSizeChangedEventArgs e);
		virtual void OnKeyUp(KeyEventArgs e) {}
		virtual void OnKeyDown(KeyEventArgs e) {}
		virtual void OnMouseMove(MouseMoveEventArgs e);
		virtual void OnMouseDown(MouseButtonEventArgs e);
		virtual void OnMouseUp(MouseButtonEventArgs e);
		virtual void OnMouseWheelChanged(MouseWheelEventArgs e) {}
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
	private:
		friend class UIElement;
		friend class Application;

		SDL_Window* _sdlWindow;
		SDL_Renderer* _renderer;
		RenderContext _renderContext;
		SDL_WindowID _id = 0;
		Ref<UIElement> _content;
		bool _isModal = false;
		bool _isClosing = false;
		bool _isClosed = true;
		bool _isActivated = false;
		bool _isRenderValid = false;
		Window* _owner = nullptr;
		std::list<Window*> _ownedWindows;
		Surface _icon;

		ValueSource _contentSource {};
	};
}