#pragma once

#include "../UIElement/UIElement.h"
#include "../Base/Media/SourcePath.h"

namespace Sgl::UIElements
{
	/// <summary>
	/// Describes how content is resized to fill its allocated space.
	/// </summary>
	enum class Stretch
	{
		None, // The content preserves its original size.
		Fill, // The content is resized to fill the destination dimensions. The aspect ratio is not preserved.
		Uniform, // The content is resized to fit in the destination dimensions while it preserves its native aspect ratio.
		UniformToFill // The content is resized to fill the destination dimensions while it preserves its native aspect ratio. If the aspect ratio of the destination rectangle differs from the source, the source content is clipped to fit in the destination dimensions.
	};

	class Image : public UIElement
	{
	public:
		Image() = default;
		Image(const Image&) = delete;
		Image(Image&& other) noexcept;

		void SetSource(SourcePath value, ValueSource source = ValueSource::Local);
		SourcePath GetSource() const { return _source; }

		void SetStretch(Stretch value, ValueSource source = ValueSource::Local);
		Stretch GetStretch() const { return _stretch; }

		void Render(RenderContext& context) override;

		static inline StyleableProperty SourceProperty { &SetSource, &GetSource };
		static inline StyleableProperty StretchProperty { &SetStretch, &GetStretch };
	protected:
		void ArrangeCore(FRect rect) override;
		void InvalidateImageTexture() { _isImageTextureValid = false; }
	private:
		void UpdateTexture();
	private:
		SourcePath _source;
		Stretch _stretch = Stretch::Uniform;

		FRect _sourceBounds {};
		Texture _sourceTexture;
		bool _isImageTextureValid = false;

		ValueSource _sourceSource {};
		ValueSource _stretchSource {};
	};
}