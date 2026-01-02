#pragma once

#include <optional>
#include <coroutine>
#include "Base/Event.h"
#include "Base/Logger.h"
#include "Base/Threading/ModalWindowAwaitable.h"
#include "Render/Surface.h"
#include "Render/TextureLoader.h"
#include "Input/TextEventArgs.h"
#include "UIElement/UIElement.h"

namespace Sgl
{
	enum class WindowDisplayMode
	{
		Window, BorderlessWindow, Fullscreen
	};

	enum class WindowState
	{
		Normal, Minimized, Maximized
	};

	struct WindowStateChangedEventArgs
	{
		WindowState State;
	};

	struct WindowPositionChangedEventArgs
	{
		int X;
		int Y;
	};

	struct WindowSizeChangedEventArgs
	{
		int Width;
		int Height;
	};

	struct CancelEventArgs
	{
		bool Cancel;
	};

	enum class DialogResult
	{
		None, Accept, Cancel
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

		Event<CancelEventHandler> Closing;
		Event<WindowEventHandler> Closed;

		/// <summary>
		/// Determines if the window should be rendered when minimized
		/// </summary>
		bool IsRenderableWhenMinimized = false;
		DialogResult DialogResult = DialogResult::None;
		TextureLoader ImageLoader;
	private:
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
	public:
		Window();
		Window(const Window&) = delete;
		Window(Window&&) = delete;
		~Window();

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
		void SetWidth(size_t value) noexcept;

		/// <summary>
		/// Gets the window width
		/// </summary>
		/// <returns>The current width in pixels</returns>
		size_t GetWidth() const noexcept;

		/// <summary>
		/// Sets the window height
		/// </summary>
		/// <param name="value">- the new height in pixels</param>
		void SetHeight(size_t value) noexcept;

		/// <summary>
		/// Gets the window height
		/// </summary>
		/// <returns>The current height in pixels</returns>
		size_t GetHeight() const noexcept;

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

		void SetAlwayOnTop(bool value = true);
		bool IsAlwayOnTop() const;

		void SetOwner(Window* owner);
		Window* GetOwner() const;

		const std::list<Window*>& GetOwnedWindows() const;

		void SetContent(Ref<UIElement> value);
		Ref<UIElement> GetContent() const;		

		void InvalidateRender() final;
		bool NeedsRendering() const final { return !_isRenderValid; }

		/// <summary>
		/// Shows the window
		/// </summary>
		void Show();

		/// <summary>
		/// Shows window as modal
		/// </summary>
		/// <param name="owner"> - the window that will own this window</param>
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

		bool IsClosed() const { return _isClosed; }

		void SetStylingRoot(IStyleHost* value) final;

		void Render(RenderContext context) override;
		void ApplyStyle() override;		
		virtual void Process();
	protected:
		void OnCursorChanged(const Cursor& cursor) override;
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

		friend class UIElement;
		friend class Application;
	public:
		static inline SglProperty ContentProperty { &SetContent, &GetContent };
	};
}