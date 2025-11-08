#pragma once

#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_image.h>
#include "Window.h"
#include "Scene/SceneManager.h"
#include "Base/Time/FPSCounter.h"
#include "Base/Localization/StringLocalizerBase.h"

namespace Sgl
{
	class Application
	{
	private:
		using ApplicationEventHandler = EventHandler<Application>;
	public:
		struct Context
		{
			Application* Current() { return Application::_current; }
			Application* operator->() { return Application::_current; }
			const Application* operator->() const { return Application::_current; }
		};

		static constexpr size_t UnlimitedFps = 0;

		Event<ApplicationEventHandler> Started;
		Event<ApplicationEventHandler> Stopped;

		Window Window;
		SceneManager SceneManager;
		StyleMap Styles;
	private:
		static inline Application* _current;

		bool _isRunning = false;
		FPSCounter _fpsCounter;
		size_t _fpsLimit = UnlimitedFps;
		TimeSpan _frameTimeLimit = TimeSpan::Zero;
		std::string _culture = "en";
		std::unique_ptr<StringLocalizerBase> _localizer;
	public:
		Application() noexcept;
		Application(const Application&) = delete;
		Application(Application&&) = delete;
		~Application();

		void SetFpsLimit(size_t value) noexcept;
		size_t GetFpsLimit() const noexcept;
		size_t GetFps() const;

		void SetCulture(const std::string& value);
		const std::string& GetCulture() const { return _culture; }

		void SetLocalizer(std::string csvFile, char delimeter = ',');
		void SetLocalizer(std::unique_ptr<StringLocalizerBase> localizer);
		const StringLocalizerBase& GetLocalizer() const;

		void Run();
		void Shutdown();
	protected:
		virtual void OnRun();
		virtual void OnShutdown();
	private:
		void HandleEvents(Scene& scene);
		void RunApp();
	};	

	inline Application::Context App;
} 