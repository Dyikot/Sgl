#include "Audio.h"
#include "Logging.h"
#include "Threading/TaskAwaitable.h"
#include "../Application.h"

#include <SDL3_mixer/SDL_mixer.h>
#include <algorithm>

namespace Sgl
{
	static constexpr float MinVolume = 0;
	static constexpr float MaxVolume = 1;

	static float Adjust(float volume)
	{
		return std::clamp(volume, MinVolume, MaxVolume);
	}

	struct AppMixer
	{
		operator MIX_Mixer*() const
		{
			return App->GetAudioMixer();
		}
	};

	static constexpr AppMixer Mixer;

	Audio::Audio(std::string_view path, bool predecode):
		_audio(MIX_LoadAudio(Mixer, path.data(), predecode))
	{
		if(!_audio)
		{
			Logging::LogError("Unable to create an Audio: {}", SDL_GetError());
		}
	}

	Audio::Audio(Audio&& other) noexcept:
		_audio(other._audio)
	{
		other._audio = nullptr;
	}

	Audio::~Audio()
	{
		MIX_DestroyAudio(_audio);
	}

	Task<Audio> Audio::LoadAsync(std::string_view path, bool predecode)
	{
		co_return co_await TaskAwaitable([path = std::string(path), predecode]()
		{
			return Audio(path, predecode);
		});
	}

	TimeSpan Audio::GetDuration() const
	{
		auto frames = MIX_GetAudioDuration(_audio);
		auto ms = MIX_AudioFramesToMS(_audio, frames);

		if(ms == -1)
		{
			Logging::LogError("Enable to get an audio duration: {}", SDL_GetError());
			return TimeSpan::Zero;
		}

		return TimeSpan::FromMilliseconds(ms);
	}

	std::string_view Audio::GetTitle() const
	{
		auto properties = MIX_GetAudioProperties(_audio);
		return SDL_GetStringProperty(properties, MIX_PROP_METADATA_TITLE_STRING, "Unknown");
	}

	std::string_view Audio::GetArtist() const
	{
		auto properties = MIX_GetAudioProperties(_audio);
		return SDL_GetStringProperty(properties, MIX_PROP_METADATA_ARTIST_STRING, "Unknown");
	}

	std::string_view Audio::GetAlbum() const
	{
		auto properties = MIX_GetAudioProperties(_audio);
		return SDL_GetStringProperty(properties, MIX_PROP_METADATA_ALBUM_STRING, "Unknown");
	}

	uint32_t Audio::GetReleaseYear() const
	{
		auto properties = MIX_GetAudioProperties(_audio);
		return SDL_GetNumberProperty(properties, MIX_PROP_METADATA_YEAR_NUMBER, 0);
	}

	MIX_Audio* Audio::GetSDLAudio() const
	{
		return _audio;
	}

	Audio& Audio::operator=(Audio&& other) noexcept
	{
		_audio = other._audio;
		other._audio = nullptr;
		return *this;
	}

	Audio::operator bool() const noexcept
	{
		return _audio;
	}

	static void OnTrackStopped(void* sender, MIX_Track* track)
	{
		auto self = static_cast<Track*>(sender);
		self->Stopped.Invoke(*self);
	}

	Track::Track():
		_track(MIX_CreateTrack(Mixer))
	{
		MIX_SetTrackStoppedCallback(_track, OnTrackStopped, this);
	}

	Track::Track(Track&& other) noexcept:
		_track(other._track)
	{
		other._track = nullptr;
		MIX_SetTrackStoppedCallback(_track, OnTrackStopped, this);
	}

	Track::~Track()
	{
		MIX_DestroyTrack(_track);
	}

	void Track::SetAudio(const Audio& audio)
	{
		MIX_SetTrackAudio(_track, audio.GetSDLAudio());
	}

	void Track::SetVolume(float volume)
	{
		MIX_SetTrackGain(_track, Adjust(volume));
	}

	float Track::GetVolume() const
	{
		return Adjust(MIX_GetTrackGain(_track));
	}

	void Track::SetPosition(TrackPosition value)
	{
		MIX_Point3D point { value.X, value.Y, value.Z };
		MIX_SetTrack3DPosition(_track, &point);
	}

	TrackPosition Track::GetPosition() const
	{
		MIX_Point3D point;
		MIX_GetTrack3DPosition(_track, &point);
		return TrackPosition(point.x, point.y, point.z);
	}

	bool Track::IsMuted() const
	{
		return GetVolume() == MinVolume;
	}

	bool Track::IsPaused() const
	{
		return MIX_TrackPaused(_track);
	}

	bool Track::IsPlaying() const
	{
		return MIX_TrackPlaying(_track);
	}
	
	void Track::SetPlaybackTime(TimeSpan value)
	{
		auto frames = MIX_TrackMSToFrames(_track, value.GetMilliseconds());
		MIX_SetTrackPlaybackPosition(_track, frames);
	}

	TimeSpan Track::GetPlaybackTime() const
	{
		auto frames = MIX_GetTrackPlaybackPosition(_track);
		auto ms = MIX_TrackFramesToMS(_track, frames);
		return TimeSpan::FromMilliseconds(std::max(0ll, ms));
	}

	TimeSpan Track::GetRemainingTime() const
	{
		auto frames = MIX_GetTrackRemaining(_track);
		auto ms = MIX_TrackFramesToMS(_track, frames);
		return TimeSpan::FromMilliseconds(std::max(0ll, ms));
	}

	void Track::Play()
	{
		MIX_PlayTrack(_track, 0);
	}

	void Track::Play(int64_t loops)
	{
		loops = std::max(-1ll, loops);

		auto properties = SDL_CreateProperties();
		SDL_SetNumberProperty(properties, MIX_PROP_PLAY_LOOPS_NUMBER, loops);
		MIX_PlayTrack(_track, properties);
		SDL_DestroyProperties(properties);
	}

	void Track::Pause()
	{
		MIX_PauseTrack(_track);
	}

	void Track::Stop()
	{
		MIX_StopTrack(_track, 0);
	}

	void Track::Resume()
	{
		MIX_ResumeTrack(_track);
	}

	Track& Track::operator=(Track&& other) noexcept
	{
		MIX_DestroyTrack(_track);
		_track = other._track;
		other._track = nullptr;
		MIX_SetTrackStoppedCallback(_track, OnTrackStopped, this);
		return *this;
	}

	void SetMixerVolume(float volume)
	{
		MIX_SetMixerGain(Mixer, Adjust(volume));
	}

	float GetMixerVolume()
	{
		return MIX_GetMixerGain(Mixer);
	}

	void RemuseAllTracks()
	{
		MIX_ResumeAllTracks(Mixer);
	}

	void PauseAllTracks()
	{
		MIX_PauseAllTracks(Mixer);
	}

	void StopAllTracks()
	{
		MIX_StopAllTracks(Mixer, 0);
	}
}