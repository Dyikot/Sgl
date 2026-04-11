#pragma once

#include <string>
#include <filesystem>

namespace Sgl
{
	class ImagePath
	{
	public:
		ImagePath() = default;
		explicit ImagePath(const std::filesystem::path& path);
		ImagePath(const std::filesystem::path& basePath, 
				  const std::filesystem::path& relativePath);
		ImagePath(const ImagePath& other);
		ImagePath(ImagePath&& other) noexcept;
		~ImagePath();

		const std::string& Get() const;
		bool IsEmpty() const;

		ImagePath& operator=(const ImagePath& other);
		ImagePath& operator=(ImagePath&& other) noexcept;
		bool operator==(const ImagePath& other) const;
	private:
		void Acquire();
		void Release();
	private:
		struct RefCountedString
		{
			std::string Value;
			uint32_t References;
		};

		RefCountedString* _data = nullptr;
	};
}