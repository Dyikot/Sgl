#pragma once

#include <optional>
#include "Render/Surface.h"
#include "Base/Size.h"
#include "Base/WindowEventArgs.h"
#include "Base/Observable/Event.h"
#include "Base/Ref.h"

namespace Sgl
{
	/// <summary>
	/// The Window class provides a high-level interface for creating and managing window,
	/// handling events, and rendering graphics. It encapsulates SDL_Window and SDL_Renderer
	/// objects and provides convenient methods for common window operations.
	/// </summary>
	class Window
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
	private:
		Surface _icon;
		bool _hasVSync = false;
	public:
		Window() noexcept;
		Window(const Window&) = delete;
		Window(Window&&) = delete;
		~Window() = default;

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
		void SetDisplayMode(DisplayMode displayMode);

		/// <summary>
		/// Gets the current display mode
		/// </summary>
		/// <returns>The current display mode</returns>
		DisplayMode GetDisplayMode() const noexcept;

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
		/// Enables or disables VSync
		/// </summary>
		/// <param name="value">- true to enable VSync, false to disable</param>
		void SetVSync(bool value = true) noexcept;

		/// <summary>
		/// Checks if VSync is enabled
		/// </summary>
		/// <returns>True if VSync is enabled, false otherwise</returns>
		bool HasVSync() const;

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
		/// Shows the window
		/// </summary>
		void Show();

		/// <summary>
		/// Hides the window
		/// </summary>
		void Hide();

		/// <summary>
		/// Checks if the window is visible
		/// </summary>
		/// <returns>- true if visible, false otherwise</returns>
		bool IsVisible() const;
	protected:
		void OnWindowStateChanged(const WindowStateEventArgs& e);
		void OnVisibilityChanged(const WindowVisibilityEventArgs& e);
		void OnPositionChanged(const WindowPositionChangedEventArgs& e);
		void OnWindowSizeChanged(const WindowSizeChangedEventArgs& e);

		friend class Application;
	};
}