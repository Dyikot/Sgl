#pragma once

#include <string_view>
#include "../Base/Primitives.h"

struct SDL_Cursor;

namespace Sgl
{
	//! @brief Represents the available system cursor types
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

	//! @brief Represents a mouse cursor, supporting system-defined cursors and custom image-based cursors. Provides static methods to control the global cursor state (visibility and appearance).
	class Cursor
	{
	public:
		//! @brief Constructs a cursor from a predefined system cursor type (e.g., Arrow, Pointer)
		//! @param systemCursor An enum value specifying the system cursor style
		Cursor(Cursors systemCursor) noexcept;

		//! @brief Loads a custom cursor from an image file at the given path. The image format must be supported by the underlying platform (typically .png or .cur).
		//! @param filePath File path to the cursor image
		//! @param hotSpot Cursor hot spot position
		Cursor(std::string_view filePath, Point hotSpot);

		Cursor(const Cursor& other);
		Cursor(Cursor&& other) noexcept;
		~Cursor() = default;

		operator SDL_Cursor* () const noexcept { return _cursor; }
		explicit operator bool() const noexcept { return _cursor; }

		Cursor& operator=(const Cursor&) = default;
		Cursor& operator=(Cursor&&) noexcept = default;
		bool operator==(const Cursor&) const = default;

	private:
		SDL_Cursor* _cursor = nullptr;
	};
}