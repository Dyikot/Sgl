#pragma once

#include <string_view>
#include "Time/TimeSpan.h"
#include "Threading/Task.h"
#include "../Base/Event.h"

struct MIX_Audio;
struct MIX_Track;

namespace Sgl
{
	class Audio
	{
	public:
		Audio() = default;
		explicit Audio(std::string_view path, bool predecode = false);
		Audio(const Audio&) = delete;
		Audio(Audio&& other) noexcept;
		~Audio();

		static Task<Audio> LoadAsync(std::string_view path, bool predecode = false);

		TimeSpan GetDuration() const;
		std::string_view GetTitle() const;
		std::string_view GetArtist() const;
		std::string_view GetAlbum() const;
		uint32_t GetReleaseYear() const;
		MIX_Audio* GetSDLAudio() const;

		Audio& operator=(const Audio&) = delete;
		Audio& operator=(Audio&& other) noexcept;
		bool operator==(const Audio&) const = default;
		explicit operator bool() const noexcept;
	private:
		MIX_Audio* _audio = nullptr;
	};

	struct TrackPosition
	{
		float X;
		float Y;
		float Z;
	};

	class Track
	{
	public:
		using TrackEventHanlder = EventHandler<Track>;
	public:
		Track();
		Track(const Track&) = delete;
		Track(Track&& other) noexcept;
		~Track();

		Event<TrackEventHanlder> Stopped;

		void SetAudio(const Audio& audio);

		void SetVolume(float volume);
		float GetVolume() const;

		void SetPosition(TrackPosition value);
		TrackPosition GetPosition() const;

		bool IsMuted() const;
		bool IsPaused() const;
		bool IsPlaying() const;

		void SetPlaybackTime(TimeSpan value);
		TimeSpan GetPlaybackTime() const;
		TimeSpan GetRemainingTime() const;

		void Play();
		void Play(int64_t loops);
		void Pause();
		void Stop();
		void Resume();

		operator bool() const noexcept { return _track != nullptr; }
		Track& operator=(const Track&) = delete;
		Track& operator=(Track&& other) noexcept;
	private:
		MIX_Track* _track = nullptr;
	};

	void SetMixerVolume(float volume);
	float GetMixerVolume();

	void RemuseAllTracks();
	void PauseAllTracks();
	void StopAllTracks();
}