#pragma once

#include <string_view>

struct SDL_Cursor;

namespace Sgl
{
    /// <summary>
    /// Systems cursors
    /// </summary>
    enum class Cursors
    {
        Arrow,
        IBeam,
        Wait,
        Crosshair,
        Progress,
        ResizeNWSE,
        ResizeNESW,
        ResizeEW,
        ResizeNS,
        Move,
        NotAllowed,
        Pointer
    };

    /// <summary>
    /// Represents a mouse cursor, supporting system-defined cursors, custom image-based cursors.
    /// Provides static methods to control the global cursor state (visibility and appearance).
    /// </summary>
    class Cursor final
    {
    public:
        /// <summary>
        /// Constructs a cursor from a predefined system cursor type (e.g., Arrow, Pointer).
        /// </summary>
        /// <param name="systemCursor"> - an enum value specifying the system cursor style.</param>
        Cursor(Cursors systemCursor) noexcept;

        /// <summary>
        /// Loads a custom cursor from an image file at the given path.
        /// The image format must be supported by the underlying platform (typically .png or .cur).
        /// </summary>
        /// <param name="path"> - file path to the cursor image.</param>
        explicit Cursor(std::string_view path);

        Cursor(const Cursor& other);
        Cursor(Cursor&& other) noexcept;
        ~Cursor() = default;

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

        Cursor& operator=(const Cursor&) = default;
        Cursor& operator=(Cursor&&) noexcept = default;
        friend bool operator==(const Cursor&, const Cursor&) = default;
        operator SDL_Cursor* () const { return _cursor; }
        explicit operator bool() const noexcept { return _cursor != nullptr; }
    private:
        SDL_Cursor* _cursor = nullptr;
    };
}