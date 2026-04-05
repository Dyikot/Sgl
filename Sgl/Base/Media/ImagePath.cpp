#include "ImagePath.h"
#include <filesystem>

namespace fs = std::filesystem;

namespace Sgl
{
	static const std::string Empty;

	ImagePath::ImagePath(std::string_view path):
		_path(new std::string(path))
	{}

	ImagePath::ImagePath(std::string_view basePath, std::string_view relativePath):
		_path(new std::string((fs::path(basePath) / relativePath).string()))
	{}

	ImagePath::ImagePath(const ImagePath& other):
		_path(new std::string(other._path ? *other._path : ""))
	{}

	ImagePath::ImagePath(ImagePath&& other) noexcept:
		_path(other._path)
	{
		other._path = nullptr;
	}

	ImagePath::~ImagePath()
	{
		delete _path;
	}

	ImagePath& ImagePath::operator=(const ImagePath& other)
	{
		if (this != &other)
		{
			delete _path;
			_path = new std::string(*other._path);
		}

		return *this;
	}

	ImagePath& ImagePath::operator=(ImagePath&& other) noexcept
	{
		if (this != &other)
		{
			delete _path;
			_path = other._path;
			other._path = nullptr;
		}

		return *this;
	}

	bool ImagePath::operator==(const ImagePath& other) const
	{
		if(_path && other._path)
		{
			return *_path == *other._path;
		}
		else
		{
			return !_path && !other._path;
		}
	}

	const std::string& ImagePath::Get() const
	{
		return _path ? *_path : Empty;
	}
}
