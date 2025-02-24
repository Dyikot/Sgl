#pragma once

#include <filesystem>
#include <iostream>
#include "Application.h"
#include "Graphic/RenderContext.h"
#include "Style/Style.h"
#include "Graphic/Surface.h"

namespace Sgl
{
	class Window;

	enum class DiplayMode
	{
		Window, BorderlessWindow, Fullscreen
	};

	enum class WindowState
	{
		Normal, Minimized, Maximized
	};

	using WindowEventHandler = EventHandler<Window, EventArgs>;
	using WindowSizeChangedEventHandler = EventHandler<Window, SizeChangedEventArgs>;

	class Window
	{
	public:
		Application& App;
		SceneManager SceneManager = Sgl::SceneManager(*this);
	protected:
		int _width = Width;
		int _height = Height;
		bool _vsyncEnabled = false;
		SDL_Window* const _this;
		std::unique_ptr<RenderContext> _renderContext;
		const Surface* _icon = nullptr;
	private:
		/* Configuration */
		static constexpr const char* Title = "Window";
		static constexpr SDL_Point Position = { SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED };
		static constexpr size_t Width = 1280;
		static constexpr size_t Height = 720;
	public:
		Window(Application& app) noexcept;
		Window(Application& app, SDL_WindowFlags flags) noexcept;
		Window(const Window&) = delete;
		Window(Window&&) = delete;
		
		~Window() noexcept { SDL_DestroyWindow(_this); }

		Event<WindowEventHandler> StateChanged;
		Event<WindowSizeChangedEventHandler> SizeChanged;
		
		void SetWidth(size_t width) noexcept;
		void SetHeight(size_t height) noexcept;
		void SetLogicalSize(size_t width, size_t height);
		void SetMaxSize(size_t width, size_t height);
		void SetMinSize(size_t width, size_t height);
		void SetTitle(std::string_view value);
		void SetPosition(SDL_Point value);
		void SetIcon(const Surface& icon);
		void SetDisplayMode(DiplayMode displayMode);
		template<typename TRenderContext> requires std::derived_from<TRenderContext, RenderContext>
		void SetRenderContext()
		{
			if(!App.IsRunning())
			{
				_renderContext = std::make_unique<TRenderContext>(
					SDL_CreateRenderer(_this, -1, SDL_RENDERER_ACCELERATED));
			}
		}

		int GetWidth() const noexcept { return _width; }
		int GetHeight() const noexcept { return _height; }
		std::pair<size_t, size_t> GetLogicalSize() const;
		std::string_view GetTitle() const { SDL_GetWindowTitle(_this); }
		SDL_Point GetPosition() const;
		WindowState GetWindowState() const;
		RenderContext& GetRenderContext() const;

		void Show();
		void Hide();
		bool IsVisible() const;
		void EnableVsync();
		void DisableVsync();
		bool IsVsyncEnable() const { return _vsyncEnabled; }

		operator SDL_Window* () const { return _this; }
	protected:
		virtual void OnSizeChanged(const SizeChangedEventArgs& e);
		virtual void OnStateChanged(const EventArgs& e);
	private:
		friend class Application;
		friend class WindowBuilder;
	};
}