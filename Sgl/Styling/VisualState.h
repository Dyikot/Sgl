#pragma once

#include <bitset>
#include <unordered_map>

#include "../Base/Event.h"

namespace Sgl
{
    //! @brief Represents a state that denotes a specific visual condition, such as "hover" or "pressed"
    class VisualState
    {
    public:
        VisualState(const VisualState&) = default;
        VisualState(VisualState&&) noexcept = default;

        //! @brief Registers a VisualState from a name. A new unique ID is assigned sequentially from 0 up to 63.
        //! @param name The string identifier for the visual state
        //! @return VisualState instance
        static VisualState Register(std::string name);

        //! @brief Returns the unique numeric identifier for this instance
        //! @return The assigned ID, guaranteed to be in the range [0, 63] for valid instances
        size_t GetId() const noexcept { return _id; }

        bool operator==(const VisualState&) const = default;

    private:
        explicit VisualState(size_t id);

    private:
        static constexpr size_t MaxId = 63;
        static inline std::unordered_map<std::string, size_t> _registry;

        size_t _id = 0;
    };

    //! @brief Represents a set of active visual states that supports change notification when modified
    class VisualStateSet
    {
    public:
        //! @brief Event handler type for visual state set changes
        using ChangedEventHandler = EventHandler<VisualStateSet&>;
        using VisualStates = std::bitset<64>;

    public:
        VisualStateSet() = default;
        VisualStateSet(const VisualStateSet&) = delete;
        VisualStateSet(VisualStateSet&&) noexcept = default;

        //! @brief Event raised when the visual state set changes
        Event<ChangedEventHandler> Changed;

        //! @brief Sets or resets a visual state in the set
        //! @param state The visual state
        //! @param value True to set, false to reset
        void Set(VisualState state, bool value = true);

        //! @brief Resets a visual state in the set
        //! @param state The visual state
        void Reset(VisualState state);

        //! @brief Determines whether the set is empty
        //! @return True if no visual states are set; otherwise, false
        bool IsEmpty() const noexcept;

        //! @brief Checks if a specific visual state is set
        //! @param state The visual state
        //! @return True if the visual state is set; otherwise, false
        bool Has(VisualState state) const;

        //! @brief Checks if all visual states in another set are set
        //! @param states The visual state set to check
        //! @return True if all visual states are set; otherwise, false
        bool Has(const VisualStateSet& states) const;

    private:
        VisualStates _states;
    };
}
