#pragma once

#include "../UIElement.h"
#include "../../Base/Media/Stretch.h"

namespace Sgl::UIElements
{
	class Image: public UIElement
	{
	private:
		std::string _source;
		FRect _sourceBounds {};
		Stretch _stretch = Stretch::Uniform;
		Texture _sourceTexture;
	public:
		Image() = default;
		Image(const Image& other);
		Image(Image&& other) noexcept;

		void SetSource(const std::string& value);
		const std::string& GetSource() const { return _source; }

		void SetStretch(Stretch value);
		Stretch GetStretch() const { return _stretch; }

		void Render(RenderContext context) override;
	protected:
		void ArrangeCore(FRect rect) override;
	public:
		static inline ObservableProperty<Image, const std::string&> SourceProperty =
			ObservableProperty<Image, const std::string&>(&SetSource, &GetSource);

		static inline ObservableProperty<Image, Stretch> StretchProperty =
			ObservableProperty<Image, Stretch>(&SetStretch, &GetStretch);
	};
}