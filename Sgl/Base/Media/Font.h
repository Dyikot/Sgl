#pragma once

#include <string>
#include <bitset>
#include <memory>
#include "Color.h"

struct TTF_Font;

namespace Sgl
{
	/// <summary>
	/// Represents a font family, encapsulating either a system-default font or a custom font loaded from a file.
	/// Internally uses reference counting to manage shared platform-specific resources safely.
	/// Supports copy, move, and comparison semantics. The default font is accessible via the static Default tag.
	/// Reference counting ensures that the underlying font resource is released only when all FontFamily instances
	/// referencing it are destroyed.
	/// </summary>
	class FontFamily final
	{
	private:
		struct FontFamilyImpl;
		struct DefaultTag {};
	public:
		/// <summary>
		/// Constructs a reference to the system's default font family.
		/// Shares a globally managed default font resource with other default instances.
		/// </summary>
		FontFamily(DefaultTag);

		/// <summary>
		/// Constructs a font family by name, typically resolving to a system-installed font.
		/// </summary>
		/// <param name="name"> - the name of the system font (e.g., "Arial", "Times New Roman").</param>
		explicit FontFamily(const std::string& name);

		/// <summary>
		/// Constructs a font family from a font file at the given path with an associated display name.
		/// </summary>
		/// <param name="path"> - file system path to the font file (e.g., .ttf or .otf).</param>
		/// <param name="name"> - logical name to associate with this font (used for identification and sharing).</param>
		FontFamily(const std::string& path, const std::string& name);

		/// <summary>
		/// Copy constructor. Increments the reference count of the shared implementation.
		/// </summary>
		FontFamily(const FontFamily& other);

		/// <summary>
		/// Move constructor. Transfers ownership of the implementation without modifying reference counts.
		/// </summary>
		FontFamily(FontFamily&& other) noexcept;

		/// <summary>
		/// Destructor. Decrements the reference count and releases the underlying resource if this was the last owner.
		/// </summary>
		~FontFamily();

		/// <summary>
		/// Tag used to construct the default system font family.
		/// </summary>
		static constexpr DefaultTag Default;

		/// <summary>
		/// Returns the source identifier of the font family.
		/// </summary>
		const std::string& GetSource() const;

		/// <summary>
		/// Copy assignment operator. Properly manages reference counts when assigning from another instance.
		/// </summary>
		FontFamily& operator=(const FontFamily& other);

		/// <summary>
		/// Move assignment operator. Efficiently transfers ownership without reference count overhead.
		/// </summary>
		FontFamily& operator=(FontFamily&& other) noexcept;

		/// <summary>
		/// Compares two FontFamily instances for equality based on their underlying implementation.
		/// Two instances are equal if they refer to the same font resource.
		/// </summary>
		friend bool operator==(const FontFamily&, const FontFamily&) = default;
	private:
		void CopyFrom(const FontFamily& other);
		void Release();
	private:
		FontFamilyImpl* _impl = nullptr;
	};

	/// <summary>
	/// Specifies style attributes that can be applied to a font.
	/// Values are defined as bit flags and can be combined using bitwise OR to represent multiple styles simultaneously
	/// (e.g., Bold | Italic for bold italic text).
	/// </summary>
	enum class FontStyles
	{
		Normal = 0,          // No special styling.
		Bold = 1,            // Thickened character strokes.
		Italic = 2,          // Slanted, cursive-style characters.
		Underline = 4,       // A line drawn beneath the text.
		Strikethrough = 8    // A horizontal line through the middle of the text.
	};

	/// <summary>
	/// Represents a set of font style attributes (bold, italic, underline, strikethrough) as a compact, immutable value.
	/// Styles are stored internally as a bitset and can be combined at construction using FontStyles enumeration values.
	/// Provides named accessors for each style and supports comparison, assignment, and conversion to an unsigned long bitmask.
	/// </summary>
	class FontStyle
	{
	public:
		/// <summary>
		/// Constructs a FontStyle with no styles enabled (equivalent to FontStyles::Normal).
		/// </summary>
		constexpr FontStyle() noexcept = default;

		/// <summary>
		/// Constructs a FontStyle by combining one or more FontStyles flags.
		/// Only values from the FontStyles enum are accepted.
		/// Example: FontStyle(FontStyles::Bold, FontStyles::Italic)
		/// </summary>
		template<std::same_as<FontStyles>... TStyles>
		constexpr FontStyle(TStyles... styles):
			_styles((static_cast<unsigned long>(styles) | ...))
		{}

		constexpr FontStyle(const FontStyle&) = default;
		constexpr FontStyle(FontStyle&&) = default;

		/// <summary>
		/// Returns true if the italic style is enabled.
		/// </summary>
		constexpr bool IsItalic() const noexcept
		{
			return _styles[0];
		}

		/// <summary>
		/// Returns true if the bold style is enabled.
		/// </summary>
		constexpr bool IsBold() const noexcept
		{
			return _styles[1];
		}

		/// <summary>
		/// Returns true if underline is enabled.
		/// </summary>
		constexpr bool HasUnderline() const noexcept
		{ 
			return _styles[2];
		}

		/// <summary>
		/// Returns true if strikethrough is enabled.
		/// </summary>
		constexpr bool HasStrikethrough() const noexcept
		{
			return _styles[3]; 
		}

		constexpr FontStyle& operator=(const FontStyle&) = default;
		constexpr FontStyle& operator=(FontStyle&&) = default;

		/// <summary>
		/// Compares two FontStyle instances for equality based on their underlying bit representation.
		/// </summary>
		friend bool operator==(const FontStyle&, const FontStyle&) = default;

		/// <summary>
		/// Implicitly converts the FontStyle to its underlying bitmask representation (unsigned long),
		/// where bits correspond to FontStyles values (bit 0 = Italic, bit 1 = Bold, etc.).
		/// </summary>
		operator unsigned long() const noexcept { return _styles.to_ulong(); }
	private:
		std::bitset<4> _styles;
	};

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
	class FontImpl final
	{
	public:
		/// <summary>
		/// Constructs an empty FontImpl with no associated font.
		/// </summary>
		FontImpl() = default;

		/// <summary>
		/// Constructs a font instance from the given font family and point size.
		/// Loads or retrieves the underlying TTF_Font using the specified family and size.
		/// </summary>
		/// <param name="fontFamily"> - the font family to use.</param>
		/// <param name="size"> - the font size in points.</param>
		FontImpl(const FontFamily& fontFamily, float size);

		FontImpl(const FontImpl&) = delete;

		/// <summary>
		/// Move constructor. Transfers ownership of the underlying TTF_Font resource.
		/// </summary>
		FontImpl(FontImpl&& other) noexcept;

		/// <summary>
		/// Destructor. Releases the underlying TTF_Font if owned.
		/// </summary>
		~FontImpl();

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

		FontImpl& operator=(const FontImpl&) = delete;

		/// <summary>
		/// Move assignment operator. Transfers ownership of the TTF_Font resource.
		/// </summary>
		FontImpl& operator=(FontImpl&& other) noexcept;

		/// <summary>
		/// Implicit conversion to the underlying TTF_Font pointer for direct use with SDL_ttf APIs.
		/// </summary>
		operator TTF_Font* () const { return _font; }
	private:
		TTF_Font* _font = nullptr;
	};
}