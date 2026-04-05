#pragma once

#include <string>
#include <string_view>

namespace Sgl
{
	class ImagePath
	{
	public:
		ImagePath() = default;
		explicit ImagePath(std::string_view path);
		ImagePath(std::string_view basePath, std::string_view relativePath);
		ImagePath(const ImagePath& other);
		ImagePath(ImagePath&& other) noexcept;
		~ImagePath();

		ImagePath& operator=(const ImagePath& other);
		ImagePath& operator=(ImagePath&& other) noexcept;
		bool operator==(const ImagePath& other) const;

		const std::string& Get() const;
	private:
		std::string* _path = nullptr;
	};
}