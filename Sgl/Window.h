#pragma once

#include <optional>
#include "Render/Renderer.h"
#include "Render/Surface.h"
#include "Base/Size.h"
#include "Base/WindowEventArgs.h"
#include "Base/Events/Event.h"

namespace Sgl
{
	using std::shared_ptr;

	class Window
	{
	public:
		using WindowStateEventHandler = EventHandler<Window, WindowStateEventArgs>;

		Event<WindowStateEventHandler> WindowStateChanged;
		bool IsRenderableWhenMinimized = false;
	protected:
		SDL_Window* _window;
		SDL_Renderer* _renderer;
	private:
		shared_ptr<Surface> _icon;
		bool _hasVSync = false;
	public:
		Window() noexcept;
		Window(const Window&) = delete;
		Window(Window&&) = delete;
		~Window() noexcept;
		
		SDL_Window* GetSDL_Window() const noexcept;

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

		void SetIcon(shared_ptr<Surface> icon);
		shared_ptr<Surface> GetIcon() const;

		void SetVSync(bool value) noexcept;
		bool HasVSync() const;

		void SetResizable(bool value) noexcept;
		bool IsResizable() const;

		void Show();
		void Hide();
		bool IsVisible() const;
	protected:
		void OnWindowStateChanged(const WindowStateEventArgs& e);
		// TODO: Add other window events

		friend class Application;
		friend class Renderer;
	};
}