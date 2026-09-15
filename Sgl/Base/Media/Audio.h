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
	//! @brief Represents a 3D point in space, typically used for spatial/positional audio calculations
	struct Point3D
	{
		float X;
		float Y;
		float Z;
	};

	//! @brief Manages audio mixing
	class Mixer
	{
	public:
		Mixer();
		Mixer(const Mixer&) = delete;
		Mixer(Mixer&&) = delete;
		~Mixer();

		//! @brief Sets the master volume for the mixer
		//! @param volume The volume level, typically between 0.0f (silent) and 1.0f (max)
		void SetVolume(float volume);

		//! @brief Gets the current master volume of the mixer
		//! @return The current master volume level
		float GetVolume();

		//! @brief Resumes playback for all currently paused audio tracks managed by this mixer
		void ResumeAllPlayers();

		//! @brief Pauses playback for all currently playing audio tracks managed by this mixer
		void PauseAllPlayers();

		//! @brief Stops playback for all currently playing audio tracks managed by this mixer
		void StopAllPlayers();

		//! @brief Gets the underlying SDL3 mixer pointer
		//! @return A pointer to the native MIX_Mixer object
		MIX_Mixer* GetSDLMixer() const;

	private:
		MIX_Mixer* _mixer {};
	};

	//! @brief Represents a loaded audio resource (e.g., sound effect or music track). Uses reference counting.
	class Audio
	{
	public:
		//! @brief Constructor. Uses application mixer.
		//! @param source The file path of the audio source
		//! @param predecode If set to true, the audio is fully decoded into memory upon loading; otherwise, it may be streamed
		Audio(std::string_view source, bool predecode = false);

		//! @brief Constructor
		//! @param mixer The mixer to associate this audio with
		//! @param source The file path of the audio source
		//! @param predecode If set to true, the audio is fully decoded into memory upon loading; otherwise, it may be streamed
		Audio(Mixer& mixer, std::string_view source, bool predecode = false);

		Audio(const Audio&) = delete;
		Audio(Audio&& other) noexcept;
		~Audio();

		//! @brief Asynchronously loads an audio resource from the specified path
		//! @param source The file path of the audio source
		//! @param predecode If set to true, the audio is fully decoded into memory upon loading
		//! @return A task representing the asynchronous load operation, which yields an Audio instance
		static Task<Audio> LoadAsync(std::string_view source, bool predecode = false);

		//! @brief Asynchronously loads an audio resource from the specified path using a specific mixer
		//! @param mixer The mixer to associate the loaded audio with
		//! @param source The file path of the audio source
		//! @param predecode If set to true, the audio is fully decoded into memory upon loading
		//! @return A task representing the asynchronous load operation, which yields an Audio instance
		static Task<Audio> LoadAsync(Mixer& mixer, std::string_view path, bool predecode = false);

		//! @brief Checks whether the audio data has been successfully loaded and is ready for playback
		//! @return True if the audio is loaded; otherwise, false
		bool IsLoaded() const;

		//! @brief Gets the total playback duration of the audio track
		//! @return A TimeSpan representing the total duration
		TimeSpan GetDuration() const;

		//! @brief Gets the title metadata of the audio track
		//! @return The title of the track, or an empty view if unavailable
		std::string_view GetTitle() const;

		//! @brief Gets the artist metadata of the audio track
		//! @return The artist name, or an empty view if unavailable
		std::string_view GetArtist() const;

		//! @brief Gets the album metadata of the audio track
		//! @return The album name, or an empty view if unavailable
		std::string_view GetAlbum() const;

		//! @brief Gets the release year metadata of the audio track
		//! @return The release year, or 0 if unavailable
		uint32_t GetReleaseYear() const;

		//! @brief Gets the underlying SDL3 audio data pointer
		//! @return A pointer to the native MIX_Audio object
		MIX_Audio* GetSDLAudio() const;

		Audio& operator=(const Audio&) = delete;
		Audio& operator=(Audio&& other) noexcept;
		bool operator==(const Audio&) const = default;

	private:
		MIX_Audio* _audio {};
	};

	//! @brief Represents an active audio playback instance (a track or channel)
	class AudioPlayer
	{
	public:
		using AudioPlayerEventHandler = EventHandler<AudioPlayer>;

		//! @brief Default constructor. Uses application mixer.
		AudioPlayer();

		//! @brief Constructor
		//! @param mixer The mixer to route this player's audio through
		explicit AudioPlayer(Mixer& mixer);

		AudioPlayer(const AudioPlayer&) = delete;
		AudioPlayer(AudioPlayer&& other) noexcept;
		~AudioPlayer();

		//! @brief Event raised when audio playback starts
		Event<AudioPlayerEventHandler> Started;

		//! @brief Event raised when audio playback stops
		Event<AudioPlayerEventHandler> Stopped;

		//! @brief Assigns an audio resource to this player for playback
		//! @param audio The audio resource to play
		void SetAudio(const Audio& audio);

		//! @brief Sets the playback volume for this specific player
		//! @param volume The volume level, typically between 0.0f (silent) and 1.0f (max)
		void SetVolume(float volume);

		//! @brief Gets the current playback volume of this player
		//! @return The current volume level
		float GetVolume() const;

		//! @brief Sets the 3D spatial position of the audio source for positional audio effects
		//! @param value The 3D coordinates representing the source position
		void SetPosition(Point3D value);

		//! @brief Gets the current 3D spatial position of the audio source
		//! @return The 3D coordinates of the source position
		Point3D GetPosition() const;

		//! @brief Checks whether the player is currently muted
		//! @return True if muted; otherwise, false
		bool IsMuted() const;

		//! @brief Checks whether the player is currently paused
		//! @return True if paused; otherwise, false
		bool IsPaused() const;

		//! @brief Checks whether the player is currently playing audio
		//! @return True if playing; otherwise, false
		bool IsPlaying() const;

		//! @brief Seeks to a specific playback time within the audio track
		//! @param value The time position to seek to
		void SetPlaybackTime(TimeSpan value);

		//! @brief Gets the current playback position within the audio track
		//! @return A TimeSpan representing the current playback time
		TimeSpan GetPlaybackTime() const;

		//! @brief Gets the remaining playback time for the audio track
		//! @return A TimeSpan representing the time left until playback finishes
		TimeSpan GetRemainingTime() const;

		//! @brief Starts playback of the assigned audio once
		void Play();

		//! @brief Starts playback of the assigned audio for a specified number of loops
		//! @param loops The number of times to loop the audio. Use -1 for infinite looping
		void Play(int64_t loops);

		//! @brief Pauses the current playback
		void Pause();

		//! @brief Stops the current playback and resets the playback position to the beginning
		void Stop();

		//! @brief Resumes playback that was previously paused
		void Resume();

		AudioPlayer& operator=(const AudioPlayer&) = delete;
		AudioPlayer& operator=(AudioPlayer&& other) noexcept;
		operator bool() const noexcept { return _track != nullptr; }

	private:
		MIX_Track* _track {};
	};
}