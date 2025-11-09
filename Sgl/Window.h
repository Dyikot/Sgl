#pragma once

#include <optional>
#include "Render/Surface.h"
#include "Base/Size.h"
#include "Base/Event.h"
#include "Base/Ref.h"
#include "Base/Primitives.h"
#include "Base/Time/Stopwatch.h"
#include "Input/MouseAndKeyEventArgs.h"
#include "Input/TextEventArgs.h"
#include "UI/UIElement.h"

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

	struct WindowStateEventArgs
	{
		WindowState State;
	};

	struct WindowVisibilityEventArgs
	{
		bool IsVisible;
	};

	struct WindowPositionChangedEventArgs
	{
		Point Position;
	};

	struct WindowSizeChangedEventArgs
	{
		size_t Width;
		size_t Height;
	};

	/// <summary>
	/// The Window class provides a high-level interface for creating and managing window,
	/// handling events, and rendering graphics. It encapsulates SDL_Window and SDL_Renderer
	/// objects and provides convenient methods for common window operations.
	/// </summary>
	class Window : public Renderable
	{
	private:
		using WindowStateEventHandler = EventHandler<Window, WindowStateEventArgs>;
		using WindowVisibilityEventHandler = EventHandler<Window, WindowVisibilityEventArgs>;
		using WindowPositionChangedEventHandler = EventHandler<Window, WindowPositionChangedEventArgs>;
		using WindowSizeChangedEventHandler = EventHandler<Window, WindowSizeChangedEventArgs>;
	public:
		/// <summary>
		/// Event triggered when the window's state changes (minimized, maximized, restored)
		/// </summary>
		Event<WindowStateEventHandler> WindowStateChanged;

		/// <summary>
		/// Event triggered when the window's visibility changes (shown/hidden)
		/// </summary>
		Event<WindowVisibilityEventHandler> VisibilityChanged;

		/// <summary>
		/// Event triggered when the window's position changes
		/// </summary>
		Event<WindowPositionChangedEventHandler> PositionChanged;

		/// <summary>
		/// Event triggered when the window's size changes
		/// </summary>
		Event<WindowSizeChangedEventHandler> SizeChanged;

		/// <summary>
		/// Determines if the window should be rendered when minimized
		/// </summary>
		bool IsRenderableWhenMinimized = false;
	protected:
		SDL_Window* _window;
		SDL_Renderer* _renderer;
		RenderContext _context;
		Ref<UIElement> _content;
	private:
		int _id = 0;
		Surface _icon;
		Stopwatch _stopwatch;
	public:
		Window() noexcept;
		Window(const Window&) = delete;
		Window(Window&&) = delete;
		virtual ~Window();

		/// <summary>
		/// Gets the underlying SDL window handle
		/// </summary>
		/// <returns>Pointer to the SDL_Window</returns>
		SDL_Window* GetSDLWindow() const noexcept;

		/// <summary>
		/// Gets the underlying SDL renderer handle
		/// </summary>
		/// <returns>Pointer to the SDL_Renderer</returns>
		SDL_Renderer* GetSDLRenderer() const noexcept;

		int GetId() const noexcept;

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
		/// Sets the render size
		/// </summary>
		/// <param name="size">- the logical size</param>
		void SetRenderSize(Size size) noexcept;

		/// <summary>
		/// Gets the render size
		/// </summary>
		/// <returns>The current logical size</returns>
		Size GetRenderSize() const noexcept;

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
		/// <param name="icon">- the icon surface to set</param>
		void SetIcon(Surface icon);

		/// <summary>
		/// Gets the current window icon
		/// </summary>
		/// <returns>Reference to the icon surface</returns>
		Surface GetIcon() const;

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

		void SetContent(Ref<UIElement> value);
		Ref<UIElement> GetContent() const;

		/// <summary>
		/// Shows the window
		/// </summary>
		void Show();

		/// <summary>
		/// Hides the window
		/// </summary>
		void Hide();

		void Focus();

		/// <summary>
		/// Checks if the window is visible
		/// </summary>
		/// <returns>- true if visible, false otherwise</returns>
		bool IsVisible() const;

		void Render(RenderContext context) override;
		virtual void Process(TimeSpan elapsed);
	protected:
		void OnCursorChanged(const Cursor& cursor) override;
		virtual void OnWindowStateChanged(WindowStateEventArgs& e);
		virtual void OnVisibilityChanged(WindowVisibilityEventArgs& e);
		virtual void OnPositionChanged(WindowPositionChangedEventArgs& e);
		virtual void OnWindowSizeChanged(WindowSizeChangedEventArgs& e);
		virtual void OnKeyUp(KeyEventArgs& e) {}
		virtual void OnKeyDown(KeyEventArgs& e) {}
		virtual void OnMouseMove(MouseEventArgs& e);
		virtual void OnMouseDown(MouseButtonEventArgs& e);
		virtual void OnMouseUp(MouseButtonEventArgs& e);
		virtual void OnMouseWheelChanged(MouseWheelEventArgs& e) {}
		virtual void OnTextInput(TextInputEventArgs& e) {}
		virtual void OnTextEditing(TextEditingEventArgs& e) {}
		virtual void OnMouseEnter() {}
		virtual void OnMouseLeave() {}
		virtual void OnGotFocus() {}
		virtual void OnLostFocus() {}
		virtual void OnClosing() {}
	private:
		void RenderCore();
		void ProcessCore();
		void UpdateStyleAndLayout();

		friend class Application;
	public:
		static inline ObservableProperty ContentProperty { &SetContent, &GetContent };
	};
}