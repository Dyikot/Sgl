#pragma once

#include "../UIElement/UIElement.h"

namespace Sgl
{
	/// <summary>
	/// Describes how content is resized to fill its allocated space.
	/// </summary>
	enum class Stretch
	{
		/// <summary>
		/// The content preserves its original size.
		/// </summary>
		None,

		/// <summary>
		/// The content is resized to fill the destination dimensions. The aspect ratio is not preserved.
		/// </summary>
		Fill,

		/// <summary>
		/// The content is resized to fit in the destination dimensions while it preserves its native aspect ratio.
		/// </summary>
		Uniform,

		/// <summary>
		/// The content is resized to fill the destination dimensions while it preserves its native aspect ratio. If the aspect ratio of the destination rectangle differs from the source, the source content is clipped to fit in the destination dimensions.
		/// </summary>
		UniformToFill
	};
}

namespace Sgl::UIElements
{
	class Image : public UIElement
	{
	private:
		std::string _source;
		FRect _sourceBounds {};
		Stretch _stretch = Stretch::Uniform;
		Texture _sourceTexture;
		bool _isImageTextureValid = false;
	public:
		Image() = default;
		Image(const Image& other);
		Image(Image&& other) noexcept;

		void SetSource(const std::string& value);
		const std::string& GetSource() const { return _source; }

		void SetStretch(Stretch value);
		Stretch GetStretch() const { return _stretch; }

		void Render(RenderContext context) final;
	protected:
		void ArrangeCore(FRect rect) override;
		void InvalidateImageTexture() { _isImageTextureValid = false; }
	private:
		void UpdateTexture();
	public:
		static inline SglProperty SourceProperty { &SetSource, &GetSource };
		static inline SglProperty StretchProperty { &SetStretch, &GetStretch };
	};
}