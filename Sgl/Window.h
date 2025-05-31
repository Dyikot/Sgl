#pragma once

#include <optional>
#include "Visual/Surface.h"
#include "Visual/Renderer.h"
#include "Events/Event.h"
#include "Events/WindowEvents.h"

namespace Sgl
{
	class Window
	{
	public:
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
		void SetHeight(size_t value) noexcept;
		void SetLogicalSize(size_t width, size_t height) noexcept;
		void SetMaxSize(size_t width, size_t height) noexcept;
		void SetMinSize(size_t width, size_t height) noexcept;
		void SetTitle(const std::string& value) noexcept;
		void SetPosition(SDL_Point value) noexcept;
		void SetIcon(std::string_view path);
		void SetDisplayMode(DisplayMode displayMode);

		size_t GetWidth() const noexcept;
		size_t GetHeight() const noexcept;
		size_t GetMaxWidth() const noexcept;
		size_t GetMaxHeight() const noexcept;
		size_t GetMinWidth() const noexcept;
		size_t GetMinHeight() const noexcept;
		size_t GetLogicalWidth() const noexcept;
		size_t GetLogicalHeight() const noexcept;
		std::string GetTitle() const noexcept;
		SDL_Point GetPosition() const noexcept;
		DisplayMode GetDisplayMode() const noexcept;
		WindowState GetWindowState() const noexcept;
		
		void EnableVSync() noexcept;
		void DisableVSync() noexcept;

		void EnableResizable() noexcept;
		void DisableResizable() noexcept;

		void Show();
		void Hide();
		bool IsVisible() const;
		bool IsResizable() const;
		bool IsVSyncEnable() const;

		operator SDL_Window* () const noexcept { return _window; }
	protected:
		void OnWindowStateChanged(const WindowStateEventArgs& e)
		{
			WindowStateChanged.TryRaise(*this, e);
		}
		// TODO: Add other window events
	private:
		void SetVSync(bool value);
		void SetResize(bool value);

		friend class Application;
		friend class Renderer;
	};
}