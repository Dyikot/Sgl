#pragma once

#include <filesystem>

struct TTF_Font;

namespace Sgl
{
	/// <summary>
	/// Represents a font family, encapsulating either a system-default font or a custom font loaded from a file.
	/// </summary>
	class FontFamily
	{
	public:
		/// <summary>
		/// Constructs a font family by name, typically resolving to a system-installed font.
		/// </summary>
		/// <param name="familyName"> - the name of the system font (e.g., "Arial", "Times New Roman").</param>
		explicit FontFamily(const std::string& familyName);

		/// <summary>
		/// Constructs a font family from a font file at the given path with an associated display name.
		/// </summary>
		/// <param name="basePath"> - the base path that is used to resolve familyName</param>
		/// <param name="familyName"> - the name of the system font (e.g., "Arial", "Times New Roman").</param>
		FontFamily(const std::filesystem::path& basePath, const std::string& familyName);

		FontFamily(const FontFamily& other);
		FontFamily(FontFamily&& other) noexcept;

		/// <summary>
		/// Default font family.
		/// </summary>
		static const FontFamily Default;

		/// <summary>
		/// Returns the file path of the font family.
		/// </summary>
		std::string_view GetSource() const;

		/// <summary>
		/// Returns the name of the font family.
		/// </summary>
		std::string_view GetName() const;

		FontFamily& operator=(FontFamily other);
		FontFamily& operator=(FontFamily&& other) noexcept;
		bool operator==(const FontFamily&) const = default;
	private:
		const std::string* _source;
		uint32_t _nameLength;
	};

	/// <summary>
	/// Specifies style attributes that can be applied to a font.
	/// Values are defined as bit flags and can be combined using bitwise OR to represent multiple styles simultaneously
	/// (e.g., Bold | Italic for bold italic text).
	/// </summary>
	enum class FontStyle
	{
		Normal		  =	0x0,   // No special styling.
		Bold		  =	0x1,   // Thickened character strokes.
		Italic        =	0x2,   // Slanted, cursive-style characters.
		Underline	  =	0x4,   // A line drawn beneath the text.
		Strikethrough = 0x8    // A horizontal line through the middle of the text.
	};

	constexpr FontStyle operator|(FontStyle left, FontStyle right) noexcept
	{
		return FontStyle(static_cast<int>(left) | static_cast<int>(right));
	}

	/// <summary>
	/// Specifies the direction in which content flows within a layout container.
	/// Used to control the arrangement of UI elements or text progression.
	/// </summary>
	enum class FlowDirection
	{
		LeftToRight,   // Content flows from left to right (standard for Latin scripts).
		RightToLeft,   // Content flows from right to left (used in Arabic, Hebrew, etc.).
		BottomToTop,   // Content flows from bottom to top.
		TopToBottom    // Content flows from top to bottom (common in vertical layouts or East Asian contexts).
	};

	/// <summary>
	/// Determines whether text should be wrapped to the next line when it exceeds the available width.
	/// </summary>
	enum class TextWrapping
	{
		NoWrap, // Text continues on a single line and may be clipped if it overflows.
		Wrap    // Text automatically breaks into multiple lines to fit within the available width.
	};

	/// <summary>
	/// Controls the horizontal alignment of text within its layout bounds.
	/// </summary>
	enum class TextAlignment
	{
		Left,   // Text is aligned to the left edge.
		Center, // Text is centered horizontally.
		Right   // Text is aligned to the right edge.
	};

	/// <summary>
	/// Specifies the rendering quality mode for font rasterization.
	/// Affects visual appearance, performance, and anti-aliasing behavior.
	/// </summary>
	enum class FontQuality
	{
		Blended, // High-quality anti-aliased rendering with alpha blending (smooth edges, slower).
		Solid,   // Fast monochrome rendering without anti-aliasing (sharp but pixelated).
		Shaded,  // Anti-aliased using a palette-based approach (good balance of quality and speed).
		LCD      // Subpixel rendering optimized for LCD screens (crisp text on RGB stripe displays).
	};

	/// <summary>
	/// A low-level, non-copyable wrapper around an SDL_ttf font object (TTF_Font*).
	/// Encapsulates font rendering settings such as size, style, outline, text alignment, and flow direction.
	/// Internally manages the lifetime of the TTF_Font resource and provides safe move semantics.
	/// </summary>
	class TrueTypeFont final
	{
	public:
		/// <summary>
		/// Constructs an empty TrueTypeFont with no associated font.
		/// </summary>
		TrueTypeFont() = default;

		/// <summary>
		/// Constructs a font instance from the given font family and point size.
		/// Loads or retrieves the underlying TTF_Font using the specified family and size.
		/// </summary>
		/// <param name="fontFamily"> - the font family to use.</param>
		/// <param name="size"> - the font size in points.</param>
		TrueTypeFont(FontFamily fontFamily, float size);

		TrueTypeFont(const TrueTypeFont&) = delete;

		/// <summary>
		/// Move constructor. Transfers ownership of the underlying TTF_Font resource.
		/// </summary>
		TrueTypeFont(TrueTypeFont&& other) noexcept;

		/// <summary>
		/// Destructor. Releases the underlying TTF_Font if owned.
		/// </summary>
		~TrueTypeFont();

		/// <summary>
		/// Changes the font size.
		/// </summary>
		/// <param name="size"> - new font size in points.</param>
		void SetSize(float size);

		/// <summary>
		/// Applies a set of font styles (bold, italic, underline, strikethrough) to the font.
		/// </summary>
		/// <param name="fontStyle"> - the combined font style flags.</param>
		void SetStyle(FontStyle fontStyle);

		/// <summary>
		/// Sets the outline thickness (in pixels) for rendered glyphs.
		/// A value of 0 disables outlining.
		/// </summary>
		/// <param name="outline"> - outline width in pixels.</param>
		void SetOutline(int outline);

		/// <summary>
		/// Specifies the direction in which text flows when rendered (e.g., left-to-right or top-to-bottom).
		/// </summary>
		/// <param name="flowDirection"> - the desired text flow direction.</param>
		void SetFlowDirection(FlowDirection flowDirection);

		/// <summary>
		/// Sets the horizontal alignment for multi-line or bounded text rendering.
		/// </summary>
		/// <param name="textAlignment"> - the text alignment (left, center, or right).</param>
		void SetTextAligment(TextAlignment textAlignment);

		TrueTypeFont& operator=(const TrueTypeFont&) = delete;

		/// <summary>
		/// Move assignment operator. Transfers ownership of the TTF_Font resource.
		/// </summary>
		TrueTypeFont& operator=(TrueTypeFont&& other) noexcept;

		/// <summary>
		/// Implicit conversion to the underlying TTF_Font pointer for direct use with SDL_ttf APIs.
		/// </summary>
		operator TTF_Font* () const { return _font; }
	private:
		TTF_Font* _font = nullptr;
	};
}