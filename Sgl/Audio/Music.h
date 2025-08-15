#pragma once

#include <memory>
#include <string_view>
#include "Volume.h"
#include "../Base/Time/TimeSpan.h"

namespace Sgl
{
	class Music
	{
	private:
		std::shared_ptr<Mix_Music> _music;
	public:
		Music() = default;
		Music(std::nullptr_t);
		explicit Music(std::string_view path) noexcept;
		Music(const Music& other);
		Music(Music&& other) noexcept;
		~Music() = default;

		TimeSpan Duration() const noexcept;

		Music& operator=(Music&&) noexcept = default;
		Music& operator=(const Music&) = default;
		friend bool operator==(const Music& left, const Music& right);
		explicit operator bool() const noexcept { return _music != nullptr; }
		explicit operator Mix_Music* () const noexcept;
	};
}