#pragma once

#include "UIElement.h"

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
		Image();
		Image(Image&& other) noexcept;

		void SetSource(const std::filesystem::path& imagePath, ValueSource source = ValueSource::Local);
		void SetSource(const ImageSource& value, ValueSource source = ValueSource::Local);
		const ImageSource& GetSource() const { return _source; }

		void SetStretch(Stretch value, ValueSource source = ValueSource::Local);
		Stretch GetStretch() const { return _stretch; }

		void Render(RenderContext context) override;

		static inline StyleableProperty SourceProperty { &SetSource, &GetSource };
		static inline StyleableProperty StretchProperty { &SetStretch, &GetStretch };
	protected:
		void OnAttachedToLogicalTree() override;
		void OnDetachedFromLogicalTree() override;
		void ArrangeContent(FRect rect) override;
	private:
		void UpdateImageTexture();
	private:
		ImageSource _source;
		Stretch _stretch = Stretch::Uniform;

		FRect _sourceBounds {};
		FRect _sourceClip {};
		Texture _sourceTexture;

		ValueSource _sourceSource {};
		ValueSource _stretchSource {};
	};
}