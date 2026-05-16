#include "SourcePath.h"
#include "../StringPool.h"

namespace fs = std::filesystem;

namespace Sgl
{
	static inline std::string NormalizePath(const fs::path& path)
	{
		return path.lexically_normal().generic_string();
	}

	static StringPool PathPool;

	SourcePath::SourcePath():
		_handle(PathPool.Create(""))
	{}

	SourcePath::SourcePath(const fs::path& path)
	{
		auto normalized_path = NormalizePath(path);
		_handle = PathPool.Create(normalized_path);
	}

	SourcePath::SourcePath(const fs::path& basePath, const fs::path& relativePath)
	{
		auto normalized_path = NormalizePath(fs::path(basePath) / relativePath);
		_handle = PathPool.Create(normalized_path);
	}

	SourcePath::SourcePath(const SourcePath& other):
		_handle(other._handle)
	{}

	SourcePath::SourcePath(SourcePath&& other) noexcept:
		_handle(other._handle)
	{}

	std::string_view SourcePath::Path() const
	{
		return PathPool.Get(_handle);
	}

	SourcePath& SourcePath::operator=(const SourcePath& other)
	{
		_handle = other._handle;
		return *this;
	}

	SourcePath& SourcePath::operator=(SourcePath&& other) noexcept
	{
		_handle = other._handle;
		return *this;
	}

	bool SourcePath::operator==(const SourcePath& other) const
	{
		return _handle == other._handle;
	}
}
