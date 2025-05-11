#pragma once

#include <filesystem>
#include <iostream>
#include "Graphic/RenderContext.h"
#include "Graphic/Surface.h"
#include "Graphic/Texture.h"
#include "Events/Event.h"

namespace Sgl
{
	class Window;

	using WindowEventHandler = EventHandler<Window, EventArgs>;

	enum class DiplayMode
	{
		Window, BorderlessWindow, Fullscreen
	};

	enum class WindowState
	{
		Normal, Minimized, Maximized
	};

	class Window
	{
	public:
		bool IsRenderableWhenMinimized = false;
	private:
		SDL_Window* _widnow;
		Renderer _renderer;
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
		void SetTitle(std::string_view value) noexcept;
		void SetPosition(SDL_Point value) noexcept;
		void SetIcon(std::string_view path);
		void SetDisplayMode(DiplayMode displayMode);

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
		WindowState GetWindowState() const noexcept;
		Renderer GetRenderer() const noexcept;
		
		void EnableVSync() noexcept;
		void DisableVSync() noexcept;

		void EnableResizable() noexcept;
		void DisableResizable() noexcept;

		void Show();
		void Hide();
		bool IsVisible() const;
		bool IsResizable() const;
		bool IsVSyncEnable() const;
	private:
		void SetVSync(bool value);
		void SetResize(bool value);
	};
}