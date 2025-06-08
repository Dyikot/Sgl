#pragma once

#include <optional>
#include "Visual/Surface.h"
#include "Visual/Renderer.h"
#include "Events/Event.h"
#include "Events/WindowEventArgs.h"
#include "Data/Size.h"

namespace Sgl
{
	class Window final
	{
	public:
		using WindowStateEventHandler = EventHandler<Window, WindowStateEventArgs>;

		bool IsRenderableWhenMinimized = false;
		Event<WindowStateEventHandler> WindowStateChanged;
	private:
		SDL_Window* _window;
		SDL_Renderer* _renderer;
		bool _isVSyncEnable = false;
		std::optional<Surface> _icon;
	public:
		Window() noexcept;
		Window(const Window&) = delete;
		Window(Window&&) = delete;
		~Window() noexcept;
		
		void SetWidth(size_t value) noexcept;
		size_t GetWidth() const noexcept;

		void SetHeight(size_t value) noexcept;
		size_t GetHeight() const noexcept;

		void SetTitle(std::string_view value) noexcept;
		std::string_view GetTitle() const noexcept;

		void SetLogicalSize(Size size) noexcept;
		Size GetLogicalSize() const noexcept;

		void SetMaxSize(Size size) noexcept;
		Size GetMaxSize() const noexcept;

		void SetMinSize(Size size) noexcept;
		Size GetMinSize() const noexcept;

		void SetPosition(SDL_Point value) noexcept;
		SDL_Point GetPosition() const noexcept;

		void SetDisplayMode(DisplayMode displayMode);
		DisplayMode GetDisplayMode() const noexcept;

		void SetWindowState(WindowState state) noexcept;
		WindowState GetWindowState() const noexcept;

		void SetIcon(std::string_view path);
		const std::optional<Surface>& GetIcon() const;

		void EnableVSync() noexcept;
		void DisableVSync() noexcept;
		bool IsVSyncEnable() const;

		void EnableResizable() noexcept;
		void DisableResizable() noexcept;
		bool IsResizable() const;

		void Show();
		void Hide();
		bool IsVisible() const;

		operator SDL_Window* () const noexcept { return _window; }
	private:
		void SetVSync(bool value);
		void SetResize(bool value);

		void OnWindowStateChanged(const WindowStateEventArgs& e)
		{
			WindowStateChanged.TryRaise(*this, e);
		}
		// TODO: Add other window events

		friend class Application;
		friend class Renderer;
	};
}