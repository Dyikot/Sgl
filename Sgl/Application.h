#pragma once

#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_image.h>
#include "Window.h"
#include "Scene/SceneManager.h"
#include "Base/Time/FPSCounter.h"
#include "Base/Localization/IStringLocalizer.h"

namespace Sgl
{
	class Application
	{
	private:
		using ApplicationEventHandler = EventHandler<Application>;
	public:
		struct Context
		{
			Application* Current()
			{
				return Application::_current;
			}

			Application* operator->()
			{
				return Application::_current;
			}

			const Application* operator->() const
			{
				return Application::_current;
			}
		};

		Event<ApplicationEventHandler> Started;
		Event<ApplicationEventHandler> Stopped;

		Window Window;
		SceneManager SceneManager;
		StyleMap Styles;
	private:
		static inline Application* _current;

		bool _isRunning = false;
		FPSCounter _fpsCounter;
		std::optional<size_t> _maxFPS;
		std::optional<TimeSpan> _maxFrameTime;
		std::string _culture = "en";
		std::unique_ptr<IStringLocalizer>  _localizer;
	public:
		Application() noexcept;
		Application(const Application&) = delete;
		Application(Application&&) = delete;
		~Application() noexcept;

		void SetMaxFPS(size_t value) noexcept;
		auto GetMaxFPS() const noexcept { return _maxFPS; }
		size_t GetFPS() const { return _fpsCounter.GetFps(); }

		void SetCulture(std::string value);
		const std::string& GetCulture() const { return _culture; }

		void SetLocalizer(std::string csvFile, char delimeter = ',');
		void SetLocalizer(Func<std::unique_ptr<IStringLocalizer>, std::string> localizerFactory);
		const IStringLocalizer& GetLocalizer() const;

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