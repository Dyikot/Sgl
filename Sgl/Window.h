#pragma once

#include <filesystem>
#include <iostream>
#include "Application.h"
#include "Graphic/RenderContext.h"
#include "Graphic/Surface.h"
#include "Graphic/Texture.h"

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
		struct Configuration
		{
			const char* Title = "Window";
			SDL_Point Position = { SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED };
			size_t Width = 1280;
			size_t Height = 720;
			SDL_WindowFlags Flags = SDL_WINDOW_HIDDEN;
		};

		Application& App;
		SceneManager SceneManager = Sgl::SceneManager(*this);
		AnyMap<std::string> Resources;
		bool CanRenderInMinimizedMode = false;
	private:
		SDL_Window* _this;
		SDL_Renderer* _renderer;
		bool _vsyncEnabled = false;
		std::optional<Surface> _icon;
	public:
		Window(Application& app) noexcept;
		Window(Application& app, const Configuration& config) noexcept;
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
		std::string_view GetTitle() const noexcept;
		SDL_Point GetPosition() const noexcept;
		WindowState GetWindowState() const;
		RenderContext GetRenderContext() const noexcept;

		void Show();
		void Hide();
		bool IsVisible() const;
		void EnableVsync();
		void EnableResize();
		void DisableVsync();
		void DisableResize();
		bool IsResizable() const;
		bool IsVsyncEnable() const;

		operator SDL_Window* () const { return _this; }
	private:
		friend class Application;
	};
}