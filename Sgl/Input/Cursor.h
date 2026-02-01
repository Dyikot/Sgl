#pragma once

#include <SDL3/SDL_mouse.h>
#include <string_view>

namespace Sgl
{
    /// <summary>
    /// Represents a mouse cursor, supporting system-defined cursors, custom image-based cursors, or raw SDL_Cursor handles.
    /// Internally uses reference counting to safely share cursor resources across instances.
    /// Provides static methods to control the global cursor state (visibility and appearance).
    /// </summary>
    class Cursor final
    {
    public:
        /// <summary>
        /// Constructs a cursor from a predefined system cursor type (e.g., Arrow, Pointer).
        /// </summary>
        /// <param name="systemCursor"> - an SDL_SystemCursor enum value specifying the system cursor style.</param>
        Cursor(SDL_SystemCursor systemCursor) noexcept;

        /// <summary>
        /// Loads a custom cursor from an image file at the given path.
        /// The image format must be supported by the underlying platform (typically .png or .cur).
        /// </summary>
        /// <param name="path"> - file path to the cursor image.</param>
        explicit Cursor(std::string_view path);

        /// <summary>
        /// Takes ownership of an externally created SDL_Cursor*.
        /// The Cursor assumes responsibility for its lifetime via reference counting.
        /// </summary>
        /// <param name="sdlCursor"> - a valid SDL_Cursor pointer created by SDL_CreateCursor or similar.</param>
        explicit Cursor(SDL_Cursor* sdlCursor);

        /// <summary>
        /// Copy constructor. Shares the underlying cursor resource; increments the reference count.
        /// </summary>
        Cursor(const Cursor& other);

        /// <summary>
        /// Move constructor. Transfers ownership of the cursor resource without affecting reference counts.
        /// </summary>
        Cursor(Cursor&& other) noexcept;

        /// <summary>
        /// Destructor. Decrements the reference count and releases the underlying SDL cursor if this was the last owner.
        /// </summary>
        ~Cursor();

        /// <summary>
        /// Sets the current system cursor to the one represented by this instance.
        /// </summary>
        /// <param name="cursor"> - the cursor to activate.</param>
        static void Set(const Cursor& cursor);

        /// <summary>
        /// Makes the mouse cursor visible.
        /// </summary>
        static void Show();

        /// <summary>
        /// Hides the mouse cursor.
        /// </summary>
        static void Hide();

        /// <summary>
        /// Checks whether the mouse cursor is currently visible.
        /// </summary>
        /// <returns>True if the cursor is visible; otherwise, false.</returns>
        static bool IsVisible();

        /// <summary>
        /// Copy assignment operator. Shares the underlying cursor resource and updates reference counts.
        /// </summary>
        Cursor& operator=(const Cursor& other);

        /// <summary>
        /// Move assignment operator. Efficiently transfers ownership without reference count overhead.
        /// </summary>
        Cursor& operator=(Cursor&& other) noexcept;

        /// <summary>
        /// Compares two Cursor instances for equality based on their underlying SDL cursor handle.
        /// </summary>
        friend bool operator==(const Cursor&, const Cursor&) = default;

        /// <summary>
        /// Explicit conversion to bool to check if the cursor is valid (non-null).
        /// </summary>
        explicit operator bool() const noexcept
        {
            return _cursor != nullptr;
        }
    private:
        void CopyFrom(const Cursor& other);
        void Destroy();
    private:
        struct CursorImpl;
        CursorImpl* _cursor = nullptr;
    };

    class Cursors
    {
    public:
        static constexpr SDL_SystemCursor Arrow = SDL_SYSTEM_CURSOR_DEFAULT;
        static constexpr SDL_SystemCursor IBeam = SDL_SYSTEM_CURSOR_TEXT;
        static constexpr SDL_SystemCursor Wait = SDL_SYSTEM_CURSOR_WAIT;
        static constexpr SDL_SystemCursor Crosshair = SDL_SYSTEM_CURSOR_CROSSHAIR;
        static constexpr SDL_SystemCursor Progress = SDL_SYSTEM_CURSOR_PROGRESS;
        static constexpr SDL_SystemCursor ResizeNWSE = SDL_SYSTEM_CURSOR_NWSE_RESIZE;
        static constexpr SDL_SystemCursor ResizeNESW = SDL_SYSTEM_CURSOR_NESW_RESIZE;
        static constexpr SDL_SystemCursor ResizeEW = SDL_SYSTEM_CURSOR_EW_RESIZE;
        static constexpr SDL_SystemCursor ResizeNS = SDL_SYSTEM_CURSOR_NS_RESIZE;
        static constexpr SDL_SystemCursor Move = SDL_SYSTEM_CURSOR_MOVE;
        static constexpr SDL_SystemCursor NotAllowed = SDL_SYSTEM_CURSOR_NOT_ALLOWED;
        static constexpr SDL_SystemCursor Pointer = SDL_SYSTEM_CURSOR_POINTER;
    };
}