#pragma once

#include <string>

#include "../Event.h"
#include "../Time/TimeSpan.h"
#include "../Threading/Task.h"

struct MIX_Audio;
struct MIX_Track;
struct MIX_Mixer;

namespace Sgl::Media
{
	/// <summary>
	/// Represents a 3D point in space, typically used for spatial/positional audio calculations
	/// </summary>
	struct Point3D
	{
		float X;
		float Y;
		float Z;
	};

	/// <summary>
	/// Manages audio mixing
	/// </summary>
	class Mixer
	{
	public:
		Mixer();
		Mixer(const Mixer&) = delete;
		Mixer(Mixer&&) = delete;
		~Mixer();

		/// <summary>
		/// Sets the master volume for the mixer
		/// </summary>
		/// <param name="volume">The volume level, typically between 0.0f (silent) and 1.0f (max)</param>
		void SetVolume(float volume);

		/// <summary>
		/// Gets the current master volume of the mixer
		/// </summary>
		/// <returns>The current master volume level</returns>
		float GetVolume();

		/// <summary>
		/// Resumes playback for all currently paused audio tracks managed by this mixer
		/// </summary>
		void ResumeAllPlayers();

		/// <summary>
		/// Pauses playback for all currently playing audio tracks managed by this mixer
		/// </summary>
		void PauseAllPlayers();

		/// <summary>
		/// Stops playback for all currently playing audio tracks managed by this mixer
		/// </summary>
		void StopAllPlayers();

		/// <summary>
		/// Gets the underlying SDL3 mixer pointer
		/// </summary>
		/// <returns>A pointer to the native MIX_Mixer object</returns>
		MIX_Mixer* GetSDLMixer() const;

	private:
		MIX_Mixer* _mixer {};
	};

	/// <summary>
	/// Represents a loaded audio resource (eg, sound effect or music track). Uses reference counting.
	/// </summary>
	class Audio
	{
	public:
		/// <summary>
		/// Constructor. Uses application mixer.
		/// </summary>
		/// <param name="source">The file path of the audio source</param>
		/// <param name="predecode">If set to true, the audio is fully decoded into memory upon loading; otherwise, it may be streamed</param>
		Audio(std::string_view source, bool predecode = false);

		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="mixer">The mixer to associate this audio with</param>
		/// <param name="source">The file path of the audio source</param>
		/// <param name="predecode">If set to true, the audio is fully decoded into memory upon loading; otherwise, it may be streamed</param>
		Audio(Mixer& mixer, std::string_view source, bool predecode = false);

		Audio(const Audio&) = delete;
		Audio(Audio&& other) noexcept;
		~Audio();

		/// <summary>
		/// Asynchronously loads an audio resource from the specified path
		/// </summary>
		/// <param name="source">The file path of the audio source</param>
		/// <param name="predecode">If set to true, the audio is fully decoded into memory upon loading</param>
		/// <returns>A task representing the asynchronous load operation, which yields an Audio instance</returns>
		static Task<Audio> LoadAsync(std::string_view source, bool predecode = false);

		/// <summary>
		/// Asynchronously loads an audio resource from the specified path using a specific mixer
		/// </summary>
		/// <param name="mixer">The mixer to associate the loaded audio with</param>
		/// <param name="source">The file path of the audio source</param>
		/// <param name="predecode">If set to true, the audio is fully decoded into memory upon loading</param>
		/// <returns>A task representing the asynchronous load operation, which yields an Audio instance</returns>
		static Task<Audio> LoadAsync(Mixer& mixer, std::string_view path, bool predecode = false);

		/// <summary>
		/// Checks whether the audio data has been successfully loaded and is ready for playback
		/// </summary>
		/// <returns>true if the audio is loaded; otherwise, false</returns>
		bool IsLoaded() const;

		/// <summary>
		/// Gets the total playback duration of the audio track
		/// </summary>
		/// <returns>A TimeSpan representing the total duration</returns>
		TimeSpan GetDuration() const;

		/// <summary>
		/// Gets the title metadata of the audio track
		/// </summary>
		/// <returns>The title of the track, or an empty view if unavailable</returns>
		std::string_view GetTitle() const;

		/// <summary>
		/// Gets the artist metadata of the audio track
		/// </summary>
		/// <returns>The artist name, or an empty view if unavailable</returns>
		std::string_view GetArtist() const;

		/// <summary>
		/// Gets the album metadata of the audio track
		/// </summary>
		/// <returns>The album name, or an empty view if unavailable</returns>
		std::string_view GetAlbum() const;

		/// <summary>
		/// Gets the release year metadata of the audio track
		/// </summary>
		/// <returns>The release year, or 0 if unavailable</returns>
		uint32_t GetReleaseYear() const;

		/// <summary>
		/// Gets the underlying SDL3 audio data pointer
		/// </summary>
		/// <returns>A pointer to the native MIX_Audio object</returns>
		MIX_Audio* GetSDLAudio() const;

		Audio& operator=(const Audio&) = delete;
		Audio& operator=(Audio&& other) noexcept;
		bool operator==(const Audio&) const = default;
	private:
		MIX_Audio* _audio {};
	};

	/// <summary>
	/// Represents an active audio playback instance (a track or channel)
	/// </summary>
	class AudioPlayer
	{
	public:
		using AudioPlayerEventHanlder = EventHandler<AudioPlayer>;
	public:
		/// <summary>
		/// Default constructor. Uses appication mixer.
		/// </summary>
		AudioPlayer();

		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="mixer">The mixer to route this player's audio through</param>
		explicit AudioPlayer(Mixer& mixer);

		AudioPlayer(const AudioPlayer&) = delete;
		AudioPlayer(AudioPlayer&& other) noexcept;
		~AudioPlayer();

		/// <summary>
		/// Event raised when audio playback starts
		/// </summary>
		Event<AudioPlayerEventHanlder> Started;

		/// <summary>
		/// Event raised when audio playback stops
		/// </summary>
		Event<AudioPlayerEventHanlder> Stopped;

		/// <summary>
		/// Assigns an audio resource to this player for playback
		/// </summary>
		/// <param name="audio">The audio resource to play</param>
		void SetAudio(const Audio& audio);

		/// <summary>
		/// Sets the playback volume for this specific player
		/// </summary>
		/// <param name="volume">The volume level, typically between 0.0f (silent) and 1.0f (max)</param>
		void SetVolume(float volume);

		/// <summary>
		/// Gets the current playback volume of this player
		/// </summary>
		/// <returns>The current volume level</returns>
		float GetVolume() const;

		/// <summary>
		/// Sets the 3D spatial position of the audio source for positional audio effects
		/// </summary>
		/// <param name="value">The 3D coordinates representing the source position</param>
		void SetPosition(Point3D value);

		/// <summary>
		/// Gets the current 3D spatial position of the audio source
		/// </summary>
		/// <returns>The 3D coordinates of the source position</returns>
		Point3D GetPosition() const;

		/// <summary>
		/// Checks whether the player is currently muted
		/// </summary>
		/// <returns>true if muted; otherwise, false</returns>
		bool IsMuted() const;

		/// <summary>
		/// Checks whether the player is currently paused
		/// </summary>
		/// <returns>true if paused; otherwise, false</returns>
		bool IsPaused() const;

		/// <summary>
		/// Checks whether the player is currently playing audio
		/// </summary>
		/// <returns>true if playing; otherwise, false</returns>
		bool IsPlaying() const;

		/// <summary>
		/// Seeks to a specific playback time within the audio track
		/// </summary>
		/// <param name="value">The time position to seek to</param>
		void SetPlaybackTime(TimeSpan value);

		/// <summary>
		/// Gets the current playback position within the audio track
		/// </summary>
		/// <returns>A <see cref="TimeSpan"/> representing the current playback time</returns>
		TimeSpan GetPlaybackTime() const;

		/// <summary>
		/// Gets the remaining playback time for the audio track
		/// </summary>
		/// <returns>A <see cref="TimeSpan"/> representing the time left until playback finishes</returns>
		TimeSpan GetRemainingTime() const;

		/// <summary>
		/// Starts playback of the assigned audio once
		/// </summary>
		void Play();

		/// <summary>
		/// Starts playback of the assigned audio for a specified number of loops
		/// </summary>
		/// <param name="loops">The number of times to loop the audio Use -1 for infinite looping</param>
		void Play(int64_t loops);

		/// <summary>
		/// Pauses the current playback
		/// </summary>
		void Pause();

		/// <summary>
		/// Stops the current playback and resets the playback position to the beginning
		/// </summary>
		void Stop();

		/// <summary>
		/// Resumes playback that was previously paused
		/// </summary>
		void Resume();

		AudioPlayer& operator=(const AudioPlayer&) = delete;
		AudioPlayer& operator=(AudioPlayer&& other) noexcept;
		operator bool() const noexcept { return _track != nullptr; }
	private:
		MIX_Track* _track {};
	};
}