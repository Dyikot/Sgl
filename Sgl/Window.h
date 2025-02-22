#pragma once

#include <stack>
#include <filesystem>
#include <iostream>
#include "UI/SceneStack.h"
#include "Render/RenderContext.h"
#include "Appearance/Style.h"
#include "Events/EventArgs.h"
#include "Appearance/Surface.h"

namespace Sgl
{
	enum class DiplayMode
	{
		Window, BorderlessWindow, Fullscreen
	};

	enum class WindowState
	{
		Normal, Minimized, Maximized
	};

	struct WindowConfiguration
	{
		std::string_view Title = "Window";
		SDL_Point Position = SDL_Point{ SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED };
		size_t Width = 1280;
		size_t Height = 720;
		size_t LogicalWidth = 1280;
		size_t LogicalHeight = 720;
		SDL_WindowFlags Flags = SDL_WINDOW_SHOWN;
	};

	class Window
	{
	public:		
		using WindowEventHandler = EventHandler<Window, EventArgs>;
		using WindowSizeChangedEventHandler = EventHandler<Window, SizeChangedEventArgs>;

		SceneStack Scenes;
	protected:
		SDL_Window* const _sdlWindow;
		RenderContext _renderContext;
		std::optional<Surface> _icon = std::nullopt;
		int _width;
		int _height;
		bool _isVsyncEnable = false;
	public:
		Window() noexcept;
		Window(std::string_view title,
			   SDL_Point position,
			   size_t width,
			   size_t height,
			   SDL_WindowFlags flags) noexcept;
		Window(const Window&) = delete;
		Window(Window&&) = delete;
		
		~Window() noexcept { SDL_DestroyWindow(_sdlWindow); }

		Event<WindowEventHandler> StateChanged;
		Event<WindowSizeChangedEventHandler> SizeChanged;
		
		void SetWidth(size_t width) noexcept;
		void SetHeight(size_t height) noexcept;
		void SetLogicalSize(size_t width, size_t height);
		void SetMaxSize(size_t width, size_t height);
		void SetTitle(std::string_view value);
		void SetPosition(SDL_Point value);
		void SetIcon(Surface&& icon);
		void SetIcon(const Surface& icon);
		void SetDisplayMode(DiplayMode displayMode);

		int	GetWidth() const noexcept { return _width; }
		int	GetHeight() const noexcept { return _height; }
		std::pair<size_t, size_t> GetLogicalSize() const;
		std::string_view GetTitle() const { SDL_GetWindowTitle(_sdlWindow); }
		SDL_Point GetPosition() const;
		WindowState GetWindowState() const;
		virtual RenderContext& GetRenderContext() { return _renderContext; }

		void Render();
		void Show();
		void Hide();
		void Close();
		bool IsVisible() const;
		void EnableVsync();
		void DisableVsync();
		bool IsVsyncEnable() const { _isVsyncEnable; }

		operator SDL_Window* () const { return _sdlWindow; }
	protected:
		virtual void OnSizeChanged(const SizeChangedEventArgs& e);
		virtual void OnStateChanged(const EventArgs& e);
	private:
		friend class Application;
		friend class WindowBuilder;
	};
}