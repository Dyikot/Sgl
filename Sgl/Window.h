#pragma once

#include <optional>
#include "Render/Surface.h"
#include "Base/Size.h"
#include "Base/WindowEventArgs.h"
#include "Base/Observable/Event.h"
#include "Base/SmartPointers.h"

namespace Sgl
{
	class Window
	{
	private:
		using WindowStateEventHandler = EventHandler<Window, WindowStateEventArgs>;
		using WindowVisibilityEventHandler = EventHandler<Window, WindowVisibilityEventArgs>;
		using WindowPositionChangedEventHandler = EventHandler<Window, WindowPositionChangedEventArgs>;
		using WindowSizeChangedEventHandler = EventHandler<Window, WindowSizeChangedEventArgs>;
	public:
		Event<WindowStateEventHandler> WindowStateChanged;
		Event<WindowVisibilityEventHandler> VisibilityChanged;
		Event<WindowPositionChangedEventHandler> PositionChanged;
		Event<WindowSizeChangedEventHandler> SizeChanged;

		bool IsRenderableWhenMinimized;
	protected:
		SDL_Window* _window;
		SDL_Renderer* _renderer;
	private:
		Unique<Surface> _icon;
		bool _hasVSync;
	public:
		Window() noexcept;
		Window(const Window&) = delete;
		Window(Window&&) = delete;
		~Window() noexcept;
		
		SDL_Window* GetSDL_Window() const noexcept;
		SDL_Renderer* GetRenderer() const noexcept;

		void SetWidth(size_t value) noexcept;
		size_t GetWidth() const noexcept;

		void SetHeight(size_t value) noexcept;
		size_t GetHeight() const noexcept;

		void SetTitle(std::string value) noexcept;
		std::string_view GetTitle() const noexcept;

		void SetLogicalSize(Size size) noexcept;
		Size GetLogicalSize() const noexcept;

		void SetMaxSize(Size size) noexcept;
		Size GetMaxSize() const noexcept;

		void SetMinSize(Size size) noexcept;
		Size GetMinSize() const noexcept;

		void SetPosition(Point value) noexcept;
		Point GetPosition() const noexcept;

		void SetDisplayMode(DisplayMode displayMode);
		DisplayMode GetDisplayMode() const noexcept;

		void SetWindowState(WindowState state) noexcept;
		WindowState GetWindowState() const noexcept;

		void SetIcon(Unique<Surface> icon);
		const Unique<Surface>& GetIcon() const;

		void SetVSync(bool value) noexcept;
		bool HasVSync() const;

		void SetResizable(bool value) noexcept;
		bool IsResizable() const;

		void Show();
		void Hide();
		bool IsVisible() const;
	protected:
		void OnWindowStateChanged(const WindowStateEventArgs& e);
		void OnVisibilityChanged(const WindowVisibilityEventArgs& e);
		void OnPositionChanged(const WindowPositionChangedEventArgs& e);
		void OnWindowSizeChanged(const WindowSizeChangedEventArgs& e);

		friend class Application;
	};
}