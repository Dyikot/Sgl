#pragma once

#include <string_view>
#include <filesystem>

namespace Sgl
{
	class SourcePath
	{
	public:
		SourcePath();
		explicit SourcePath(const std::filesystem::path& path);
		SourcePath(const std::filesystem::path& basePath, const std::filesystem::path& relativePath);
		SourcePath(const SourcePath& other);
		SourcePath(SourcePath&& other) noexcept;

		std::string_view Path() const;

		SourcePath& operator=(const SourcePath& other);
		SourcePath& operator=(SourcePath&& other) noexcept;
		bool operator==(const SourcePath& other) const;
	private:
		uint32_t _handle;

		friend class std::hash<SourcePath>;
	};
}

namespace std
{
	template<>
	struct hash<Sgl::SourcePath>
	{
		size_t operator()(Sgl::SourcePath path) const
		{
			return hash<uint16_t>()(path._handle);
		}
	};
}