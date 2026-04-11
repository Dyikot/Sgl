#include "ImagePath.h"

namespace fs = std::filesystem;

namespace Sgl
{
	static const std::string EmptyString;

	static inline std::string NormalizePath(const fs::path& path)
	{
		return path.lexically_normal().generic_string();
	}

	ImagePath::ImagePath(const fs::path& path):
		_data(new RefCountedString(NormalizePath(path), 1))
	{}

	ImagePath::ImagePath(const fs::path& basePath, const fs::path& relativePath):
		_data(new RefCountedString(NormalizePath(fs::path(basePath) / relativePath), 1))
	{}

	ImagePath::ImagePath(const ImagePath& other):
		_data(other._data)
	{
		Acquire();
	}

	ImagePath::ImagePath(ImagePath&& other) noexcept:
		_data(other._data)
	{
		other._data = nullptr;
	}

	ImagePath::~ImagePath()
	{
		Release();
	}

	const std::string& ImagePath::Get() const
	{
		return _data ? _data->Value : EmptyString;
	}

	bool ImagePath::IsEmpty() const
	{
		return _data == nullptr;
	}

	ImagePath& ImagePath::operator=(const ImagePath& other)
	{
		if(this != &other)
		{
			Release();
			_data = other._data;
			Acquire();
		}

		return *this;
	}

	ImagePath& ImagePath::operator=(ImagePath&& other) noexcept
	{
		if(this != &other)
		{
			Release();
			_data = other._data;
			other._data = nullptr;
		}

		return *this;
	}

	bool ImagePath::operator==(const ImagePath& other) const
	{
		if(_data && other._data)
		{
			return _data->Value == other._data->Value;
		}
		else
		{
			return !_data && !other._data;
		}
	}

	void ImagePath::Acquire()
	{
		if(_data)
		{
			++_data->References;
		}
	}

	void ImagePath::Release()
	{
		if(_data)
		{
			if(--_data->References == 0)
			{
				delete _data;
			}

			_data = nullptr;
		}
	}
}
