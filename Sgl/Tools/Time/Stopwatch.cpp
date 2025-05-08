#include "Stopwatch.h" // Assuming the declaration is in Stopwatch.h

namespace Sgl
{
    // Constructor
    Stopwatch::Stopwatch() noexcept
        : _currentIntervalStartTime(Clock::now()), // Initialize, though Start() will overwrite
          _accumulatedDuration(Duration::zero()),
          _isRunning(false)
    {
    }

    void Stopwatch::Start()
    {
        if (!_isRunning)
        {
            _currentIntervalStartTime = Clock::now();
            _isRunning = true;
        }
        // If already running, it continues from the existing _currentIntervalStartTime.
        // Some implementations might choose to throw an error or restart the interval.
        // This version mirrors the original's behavior of doing nothing if already running.
    }

    void Stopwatch::Stop() // This was effectively the original Pause()
    {
        if (_isRunning)
        {
            const auto now = Clock::now();
            _accumulatedDuration += (now - _currentIntervalStartTime);
            _isRunning = false;
            // _currentIntervalStartTime now holds the start of the just-stopped interval.
            // If Start() is called again, it will be updated.
        }
        // If not running, do nothing.
    }

    void Stopwatch::Reset() noexcept
    {
        _accumulatedDuration = Duration::zero();
        _currentIntervalStartTime = Clock::now(); // Reset start point for a potential next Start()
        _isRunning = false;
    }

    void Stopwatch::Restart()
    {
        // The original Restart did `_running = false; Reset(); Start();`
        // Since our Reset() now sets _isRunning = false, this is simpler.
        Reset();
        Start();
    }

    [[nodiscard]] Stopwatch::Duration Stopwatch::GetTotalElapsed() const
    {
        if (_isRunning)
        {
            const auto now = Clock::now();
            return _accumulatedDuration + (now - _currentIntervalStartTime);
        }
        else
        {
            return _accumulatedDuration;
        }
    }

    Stopwatch::Duration Stopwatch::RecordLapAndGetTotalElapsed() // This was the original Elapsed()
    {
        if (_isRunning)
        {
            const auto now = Clock::now();
            _accumulatedDuration += (now - _currentIntervalStartTime);
            _currentIntervalStartTime = now; // Reset for the next "lap" or segment
        }
        // If not running, it simply returns the currently accumulated duration
        // without changing any state, consistent with original behavior where
        // the update part was conditional on _running.
        return _accumulatedDuration;
    }

    [[nodiscard]] bool Stopwatch::IsRunning() const noexcept
    {
        return _isRunning;
    }

} // namespace Sgl
