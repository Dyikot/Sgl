#include "Audio.h"
#include "../Logging.h"
#include "../Threading/TaskAwaiter.h"
#include "../../Application.h"

#include <SDL3_mixer/SDL_mixer.h>
#include <algorithm>

namespace Sgl::Media
{
	static constexpr float MinVolume = 0;
	static constexpr float MaxVolume = 1;

	static float Adjust(float volume)
	{
		return std::clamp(volume, MinVolume, MaxVolume);
	}

	Mixer::Mixer()
	{
		_mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);

		if(!_mixer)
		{
			Logging::LogError("Unable to create mixer device: {}", SDL_GetError());
		}
	}

	Mixer::~Mixer()
	{
		MIX_DestroyMixer(_mixer);
	}

	void Mixer::SetVolume(float volume)
	{
		MIX_SetMixerGain(_mixer, Adjust(volume));
	}

	float Mixer::GetVolume()
	{
		return MIX_GetMixerGain(_mixer);
	}

	void Mixer::ResumeAllPlayers()
	{
		MIX_ResumeAllTracks(_mixer);
	}

	void Mixer::PauseAllPlayers()
	{
		MIX_PauseAllTracks(_mixer);
	}

	void Mixer::StopAllPlayers()
	{
		MIX_StopAllTracks(_mixer, 0);
	}

	MIX_Mixer* Mixer::GetSDLMixer() const
	{
		return _mixer;
	}

	Audio::Audio(std::string_view source, bool predecode):
		Audio(App->GetServices().GetRequired<Mixer>(), source, predecode)
	{}

	Audio::Audio(Mixer& mixer, std::string_view source, bool predecode):
		_audio(MIX_LoadAudio(mixer.GetSDLMixer(), source.data(), predecode))
	{
		if(!_audio)
		{
			Logging::LogError("Unable to load an Audio: {}", SDL_GetError());
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

	Task<Audio> Audio::LoadAsync(std::string_view source, bool predecode)
	{
		return LoadAsync(App->GetServices().GetRequired<Mixer>(), source, predecode);
	}

	Task<Audio> Audio::LoadAsync(Mixer& mixer, std::string_view source, bool predecode)
	{
		co_return co_await TaskAwaiter([&mixer, source = std::move(source), predecode]()
		{
			return Audio(source, predecode);
		});
	}

	bool Audio::IsLoaded() const
	{
		return _audio != nullptr;
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

	static void OnTrackStopped(void* sender, MIX_Track* track)
	{
		auto self = static_cast<AudioPlayer*>(sender);
		self->Stopped.Invoke(*self);
	}

	AudioPlayer::AudioPlayer():
		AudioPlayer(App->GetServices().GetRequired<Mixer>())
	{
	}

	AudioPlayer::AudioPlayer(Mixer& mixer):
		_track(MIX_CreateTrack(mixer.GetSDLMixer()))
	{
		MIX_SetTrackStoppedCallback(_track, OnTrackStopped, this);
	}

	AudioPlayer::AudioPlayer(AudioPlayer&& other) noexcept:
		_track(other._track)
	{
		other._track = nullptr;
	}

	AudioPlayer::~AudioPlayer()
	{
		MIX_DestroyTrack(_track);
	}

	void AudioPlayer::SetAudio(const Audio& audio)
	{
		MIX_SetTrackAudio(_track, audio.GetSDLAudio());
	}

	void AudioPlayer::SetVolume(float volume)
	{
		MIX_SetTrackGain(_track, Adjust(volume));
	}

	float AudioPlayer::GetVolume() const
	{
		return Adjust(MIX_GetTrackGain(_track));
	}

	void AudioPlayer::SetPosition(Point3D value)
	{
		MIX_Point3D point { value.X, value.Y, value.Z };
		MIX_SetTrack3DPosition(_track, &point);
	}

	Point3D AudioPlayer::GetPosition() const
	{
		MIX_Point3D point;
		MIX_GetTrack3DPosition(_track, &point);
		return Point3D(point.x, point.y, point.z);
	}

	bool AudioPlayer::IsMuted() const
	{
		return GetVolume() == MinVolume;
	}

	bool AudioPlayer::IsPaused() const
	{
		return MIX_TrackPaused(_track);
	}

	bool AudioPlayer::IsPlaying() const
	{
		return MIX_TrackPlaying(_track);
	}
	
	void AudioPlayer::SetPlaybackTime(TimeSpan value)
	{
		auto frames = MIX_TrackMSToFrames(_track, value.GetMilliseconds());
		MIX_SetTrackPlaybackPosition(_track, frames);
	}

	TimeSpan AudioPlayer::GetPlaybackTime() const
	{
		auto frames = MIX_GetTrackPlaybackPosition(_track);
		auto ms = MIX_TrackFramesToMS(_track, frames);
		return TimeSpan::FromMilliseconds(std::max(0ll, ms));
	}

	TimeSpan AudioPlayer::GetRemainingTime() const
	{
		auto frames = MIX_GetTrackRemaining(_track);
		auto ms = MIX_TrackFramesToMS(_track, frames);
		return TimeSpan::FromMilliseconds(std::max(0ll, ms));
	}

	void AudioPlayer::Play()
	{
		MIX_PlayTrack(_track, 0);
		Started.Invoke(*this);
	}

	void AudioPlayer::Play(int64_t loops)
	{
		loops = std::max(-1ll, loops);

		auto properties = SDL_CreateProperties();
		SDL_SetNumberProperty(properties, MIX_PROP_PLAY_LOOPS_NUMBER, loops);
		MIX_PlayTrack(_track, properties);
		SDL_DestroyProperties(properties);

		Started.Invoke(*this);
	}

	void AudioPlayer::Pause()
	{
		MIX_PauseTrack(_track);
	}

	void AudioPlayer::Stop()
	{
		MIX_StopTrack(_track, 0);
	}

	void AudioPlayer::Resume()
	{
		MIX_ResumeTrack(_track);
	}

	AudioPlayer& AudioPlayer::operator=(AudioPlayer&& other) noexcept
	{
		if(this != &other)
		{
			MIX_DestroyTrack(_track);
			_track = other._track;
			other._track = nullptr;
		}

		return *this;
	}
}