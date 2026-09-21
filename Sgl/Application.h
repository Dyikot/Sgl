#pragma once

#include "Base/Event.h"
#include "Base/ServiceLocator.h"
#include "Base/Media/ThemeMode.h"
#include "Base/Media/ThemeResources.h"
#include "Base/Localization/StringLocalizer.h"
#include "Base/Localization/LocalizationResources.h"
#include "Styling/IStyleHost.h"

namespace Sgl
{
    //! @brief Specifies when the application should shut down automatically
    enum class ShutdownMode
    {
        OnMainWindowClose, // The application exits when the main window is closed
        OnLastWindowClose, // The application exits when the last open window is closed
        OnExplicitShutdown // The application only shuts down when explicitly requested (e.g., via Shutdown())
    };

    class Window;

    //! @brief Represents the main application instance, managing windows, styling, theme, and the event loop
    class Application : public IStyleHost
    {
    public:
        struct Context;
        using ApplicationEventHandler = EventHandler<Application>;
    public:
        //! @brief Constructs an application instance
        Application() noexcept;

        Application(const Application&) = delete;
        Application(Application&&) = delete;

        //! @brief Destroys the application and releases all managed resources
        ~Application();

        //! @brief Event raised when the application has started and entered its main loop
        Event<ApplicationEventHandler> Started;

        //! @brief Event raised when the application is shutting down and the main loop has exited
        Event<ApplicationEventHandler> Stopped;

        //! @brief Event raised when the active theme variant changes
        Event<ApplicationEventHandler> ThemeVariantChanged;

        //! @brief Collection of global styles applied to all UI elements in the application
        StyleCollection Styles;

        //! @brief Theme resource provider for managing themed colors and brushes
        ThemeResources Resources;

        //! @brief Localization resources for managing text translation
        LocalizationResources Localization;

        //! @brief The primary window of the application
        Ref<Window> MainWindow;

        //! @brief Determines when the application automatically shuts down
        ShutdownMode ShutdownMode;

        //! @brief Sets the active theme variant (e.g., Light, Dark, or System). If the new value differs from the current one, the ThemeVariantChanged event is triggered. When set to System, the application automatically follows the OS-level theme preference.
        //! @param value The new theme variant to apply
        void SetThemeVariant(ThemeVariant value);

        //! @brief Gets the currently active theme variant
        //! @return The current theme variant
        ThemeVariant GetThemeVariant() const { return _themeVariant; }

        //! @brief Gets the collection of styles defined by this host
        //! @return A reference to the style collection
        StyleCollection& GetStyles() final { return Styles; }

        //! @brief Executes an action on the style collection in a read-only context
        //! @param action The callback action to execute with the immutable style collection
        void WithStyles(const Action<const StyleCollection&>& action) const final;

        //! @brief Gets the service locator
        //! @return Reference to service locator
        ServiceLocator& GetServices() { return *_services; }

        //! @brief Gets the active window
        //! @return A pointer to the active window
        Window* GetActiveWindow() { return _focusedWindow; }

        //! @brief Searches for a window by ID
        //! @param id Unique window identifier
        //! @return Pointer to a window or nullptr if not found
        Window* GetWindow(uint32_t id);

        //! @brief Gets a list of all currently open windows
        //! @return A vector containing pointers to all active windows
        const std::vector<Window*>& GetWindows() const noexcept { return _activeWindows; }

        //! @brief Starts the application's main event loop. Blocks until the application is shut down.
        void Run();

        //! @brief Requests a shutdown of the application. The main loop will exit after processing pending events.
        void Shutdown();
    protected:
        virtual void OnStarted();
        virtual void OnStopped();
        virtual void OnThemeVariantChanged();
    private:
        ThemeMode GetSystemThemeMode() const;
        void HandleInputEvents();
        void AddDefaultStyles();
        void RegisterDefaultServices();
        void PushSDLUserEvent(uint32_t type);
        void AddWindow(Window& window);
        void RemoveWindow(Window& window);
        void AttachWindow(Window& window);
        void DetachWindow(Window& window);
    private:
        static inline Application* _current;
        Window* _focusedWindow = nullptr;
        std::vector<std::pair<uint32_t, Window*>> _windowsById;
        std::vector<Window*> _activeWindows;
        ServiceLocator* _services = new ServiceLocator();
        bool _isRunning = false;
        ThemeVariant _themeVariant;

        friend class Window;
        friend class StringLocalizer;
    };

    //! @brief Provides access to the current application instance via a global context
    struct Application::Context
    {
        Application* operator->() { return Application::_current; }
        operator Application* () { return Application::_current; }
    };

    //! @brief Global access point to the current application instance. Provides convenient syntax like App->GetThemeVariant() or App.Current()->MainWindow.
    inline Application::Context App;
}