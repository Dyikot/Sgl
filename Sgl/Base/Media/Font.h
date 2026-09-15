#pragma once
#include <string>
#include <filesystem>
#include "../Ref.h"

struct TTF_Font;

namespace Sgl
{
	//! @brief Represents a font family, encapsulating either a system-default font or a custom font loaded from a file
	class FontFamily
	{
	public:
		class Impl;
	public:
		//! @brief Constructs a font family by name, typically resolving to a system-installed font
		//! @param fontFileName The file name of the system font
		explicit FontFamily(const std::string& fontFileName);

		//! @brief Constructs a font family from a font file at the given path with an associated display name
		//! @param basePath The base path that is used to resolve familyName
		//! @param fontFileName The file name of the system font
		FontFamily(const std::filesystem::path& basePath, const std::string& fontFileName);

		FontFamily(const FontFamily& other);
		FontFamily(FontFamily&& other) noexcept;
		~FontFamily();

		//! @brief Default font family
		static FontFamily GetDefault();

		//! @brief Returns the file path of the font family
		const std::filesystem::path& GetSource() const;

		//! @brief Returns the name of the font family
		const std::string& GetName() const;

		FontFamily& operator=(const FontFamily& other);
		FontFamily& operator=(FontFamily&& other) noexcept;
		bool operator==(const FontFamily& other) const;

	private:
		Ref<Impl> _impl;
	};

	//! @brief Specifies style attributes that can be applied to a font.
	//! Values are defined as bit flags and can be combined using bitwise OR to represent multiple styles simultaneously
	//! (e.g., Bold | Italic for bold italic text).
	enum class FontStyle
	{
		Normal = 0x0,   // No special styling.
		Bold = 0x1,   // Thickened character strokes.
		Italic = 0x2,   // Slanted, cursive-style characters.
		Underline = 0x4,   // A line drawn beneath the text.
		Strikethrough = 0x8    // A horizontal line through the middle of the text.
	};

	constexpr FontStyle operator|(FontStyle left, FontStyle right) noexcept
	{
		return FontStyle(static_cast<int>(left) | static_cast<int>(right));
	}

	constexpr FontStyle operator&(FontStyle left, FontStyle right) noexcept
	{
		return FontStyle(static_cast<int>(left) & static_cast<int>(right));
	}

	//! @brief Specifies the direction in which content flows within a layout container.
	//! Used to control the arrangement of UI elements or text progression.
	enum class FlowDirection
	{
		LeftToRight,   // Content flows from left to right (standard for Latin scripts).
		RightToLeft,   // Content flows from right to left (used in Arabic, Hebrew, etc.).
		BottomToTop,   // Content flows from bottom to top.
		TopToBottom    // Content flows from top to bottom (common in vertical layouts or East Asian contexts).
	};

	//! @brief Determines whether text should be wrapped to the next line when it exceeds the available width
	enum class TextWrapping
	{
		NoWrap, // Text continues on a single line and may be clipped if it overflows.
		Wrap    // Text automatically breaks into multiple lines to fit within the available width.
	};

	//! @brief Controls the horizontal alignment of text within its layout bounds
	enum class TextAlignment
	{
		Left,   // Text is aligned to the left edge.
		Center, // Text is centered horizontally.
		Right   // Text is aligned to the right edge.
	};

	//! @brief Specifies the rendering quality mode for font rasterization.
	//! Affects visual appearance, performance, and anti-aliasing behavior.
	enum class FontQuality
	{
		Blended, // High-quality anti-aliased rendering with alpha blending (smooth edges, slower).
		Solid,   // Fast monochrome rendering without anti-aliasing (sharp but pixelated).
		Shaded,  // Anti-aliased using a palette-based approach (good balance of quality and speed).
		LCD      // Subpixel rendering optimized for LCD screens (crisp text on RGB stripe displays).
	};

	//! @brief A low-level, non-copyable wrapper around an SDL_ttf font object (TTF_Font*).
	//! Encapsulates font rendering settings such as size, style, outline, text alignment, and flow direction.
	//! Internally manages the lifetime of the TTF_Font resource and provides safe move semantics.
	class Font
	{
	public:
		//! @brief Constructs an empty TrueTypeFont with no associated font
		Font() = default;

		//! @brief Constructs a font instance from the given font family and point size.
		//! Loads or retrieves the underlying TTF_Font using the specified family and size.
		//! @param fontFamily The font family to use
		//! @param size The font size in points
		Font(FontFamily fontFamily, float size);

		Font(const Font&) = delete;

		//! @brief Move constructor. Transfers ownership of the underlying TTF_Font resource
		Font(Font&& other) noexcept;

		//! @brief Destructor. Releases the underlying TTF_Font if owned
		~Font();

		//! @brief Changes the font size
		//! @param size New font size in points
		void SetSize(float size);

		//! @brief Applies a set of font styles (bold, italic, underline, strikethrough) to the font
		//! @param fontStyle The combined font style flags
		void SetStyle(FontStyle fontStyle);

		//! @brief Sets the outline thickness (in pixels) for rendered glyphs.
		//! A value of 0 disables outlining.
		//! @param outline Outline width in pixels
		void SetOutline(int outline);

		//! @brief Specifies the direction in which text flows when rendered (e.g., left-to-right or top-to-bottom)
		//! @param flowDirection The desired text flow direction
		void SetFlowDirection(FlowDirection flowDirection);

		//! @brief Sets the horizontal alignment for multi-line or bounded text rendering
		//! @param textAlignment The text alignment (left, center, or right)
		void SetTextAligment(TextAlignment textAlignment);

		Font& operator=(const Font&) = delete;

		//! @brief Move assignment operator. Transfers ownership of the TTF_Font resource
		Font& operator=(Font&& other) noexcept;

		//! @brief Implicit conversion to the underlying TTF_Font pointer for direct use with SDL_ttf APIs
		operator TTF_Font* () const { return _impl; }

	private:
		TTF_Font* _impl = nullptr;
	};
}