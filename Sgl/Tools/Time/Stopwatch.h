#pragma once // Modern include guard

#include <chrono> // Required for std::chrono types and functions

namespace Sgl
{
    class Stopwatch
    {
    public:
        // Type aliases for clarity and potentially easier changes later
        using Clock = std::chrono::steady_clock; // Use steady_clock for monotonic timing
        using TimePoint = Clock::time_point;
        using Duration = Clock::duration;      // Represents the native duration of the clock

        // Constructor
        Stopwatch() noexcept;

        // Core stopwatch controls
        void Start();                          // Starts or resumes timing.
        void Stop();                           // Stops timing and accumulates the current interval.
        void Reset() noexcept;                 // Resets accumulated time and stops the timer.
        void Restart();                        // Convenience: Resets, then starts the timer.

        // Get elapsed time
        // Returns the total accumulated duration, including the current running interval (if any).
        // This method is const and does not alter the stopwatch's state.
        [[nodiscard]] Duration GetTotalElapsed() const;

        // Mimics the original Elapsed() behavior:
        // If running, adds the current interval to the accumulated total,
        // then resets the start of the current interval (like a lap timer).
        // Returns the total accumulated duration after this operation.
        Duration RecordLapAndGetTotalElapsed();

        // Status
        [[nodiscard]] bool IsRunning() const noexcept;

        // Convenience template functions to get elapsed time in specific units
        template<typename T_Duration = std::chrono::milliseconds>
        [[nodiscard]] typename T_Duration::rep GetTotalElapsedCount() const {
            return std::chrono::duration_cast<T_Duration>(GetTotalElapsed()).count();
        }

        template<typename Rep = double, typename Period = std::ratio> // e.g., Period = std::milli for ms
        [[nodiscard]] Rep GetTotalElapsedAs() const {
            return std::chrono::duration<Rep, Period>(GetTotalElapsed()).count();
        }
        
        template<typename T_Duration = std::chrono::milliseconds>
        typename T_Duration::rep RecordLapAndGetTotalElapsedCount() {
            return std::chrono::duration_cast<T_Duration>(RecordLapAndGetTotalElapsed()).count();
        }

        template<typename Rep = double, typename Period = std::ratio>
        Rep RecordLapAndGetTotalElapsedAs() {
             return std::chrono::duration<Rep, Period>(RecordLapAndGetTotalElapsed()).count();
        }


    private:
        TimePoint _currentIntervalStartTime; // Start time of the current (or most recent) timing interval
        Duration _accumulatedDuration;       // Sum of durations of all completed intervals
        bool _isRunning;                     // True if the stopwatch is currently timing
    };

} // namespace Sgl
