#pragma once

namespace Sgl
{
    /// <summary>
    /// Represents a bitmask of modifier keys (e.g., Shift, Ctrl, Alt) active during a keyboard event.
    /// </summary>
	using KeyModifier = unsigned short;

    /// <summary>
    /// A collection of static constants representing individual keyboard modifier keys.
    /// Each modifier is assigned a unique bit flag, allowing multiple modifiers to be combined
    /// using bitwise OR (e.g., KeyModifiers::LShift | KeyModifiers::Ctrl).
    /// </summary>
    class KeyModifiers
    {
    public:
        static constexpr KeyModifier None = 0x0000u;
        static constexpr KeyModifier LShift = 0x0001u;
        static constexpr KeyModifier RShift = 0x0002u;
        static constexpr KeyModifier Level5 = 0x0004u;
        static constexpr KeyModifier LCtrl = 0x0040u;
        static constexpr KeyModifier RCtrl = 0x0080u;
        static constexpr KeyModifier LAlt = 0x0100u;
        static constexpr KeyModifier RAlt = 0x0200u;
        static constexpr KeyModifier LGui = 0x0400u;
        static constexpr KeyModifier RGui = 0x0800u;
        static constexpr KeyModifier Caps = 0x2000u;
        static constexpr KeyModifier Mode = 0x4000u;
        static constexpr KeyModifier Scroll = 0x8000u;

        static constexpr KeyModifier Ctrl = (LCtrl | RCtrl);
        static constexpr KeyModifier Shift = (LShift | RShift);
        static constexpr KeyModifier Alt = (LAlt | RAlt);
        static constexpr KeyModifier Gui = (LGui | RGui);
    };
}