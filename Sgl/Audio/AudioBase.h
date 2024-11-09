#pragma once

#include "SDL/SDL_mixer.h"

namespace Sgl
{
	class Volume
	{
	public:
		static constexpr uint8_t Max = 100;
	protected:
		uint8_t _value;
	public:
		Volume(uint8_t value):
			_value(Adjust(value))
		{}

		operator uint8_t() { return _value; }
		uint8_t operator*(Volume volume) { return _value * volume._value; }
	private:
		static uint8_t Adjust(uint8_t value) { return value > Max ? Max : value; }
	};

	class AudioBase
	{
	public:
		Volume MaxVolume;
	protected:
		static constexpr int Endless = -1;
		static constexpr int Once = 0;
	public:
		AudioBase() noexcept;
		virtual ~AudioBase() = default;

		virtual void Play() const = 0;
		virtual void SetVolume(Volume volume) = 0;
	protected:
		uint8_t ToMixVolume(Volume volume) const
		{ 
			return static_cast<double>(volume * MaxVolume) / (Volume::Max * Volume::Max)
				   * MIX_MAX_VOLUME;
		}
	};
}