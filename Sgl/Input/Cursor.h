#pragma once

#include <string_view>
#include "../Base/Primitives.h"

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
        /// <param name="filePath"> - file path to the cursor image.</param>
        /// <param name="hotSpot"> - cursor hot spot position.</param>
        Cursor(std::string_view filePath, Point hotSpot);

        Cursor(const Cursor& other);
        Cursor(Cursor&& other) noexcept;
        ~Cursor() = default;

        SDL_Cursor* GetSDLCursor() const;

        Cursor& operator=(const Cursor&) = default;
        Cursor& operator=(Cursor&&) noexcept = default;
        bool operator==(const Cursor&) const = default;
        explicit operator bool() const noexcept { return _cursor != nullptr; }
    private:
        SDL_Cursor* _cursor;
    };

    class CurrentCursorImpl
    {
    public:
        void Set(const Cursor& cursor);
        void Show();
        void Hide();
        bool IsVisible();
    };

    inline CurrentCursorImpl CurrentCursor;
}