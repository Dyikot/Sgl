#pragma once

namespace Sgl
{
	using KeyModifier = unsigned short;

    class KeyModifiers
    {
    public:
        static constexpr KeyModifier None = 0x0000u; // No modifier is applicable.
        static constexpr KeyModifier LShift = 0x0001u; // The left Shift key is down.
        static constexpr KeyModifier RShift = 0x0002u; // The right Shift key is down.
        static constexpr KeyModifier Level5 = 0x0004u; // The Level 5 Shift key is down.
        static constexpr KeyModifier LCtrl = 0x0040u; // The left Ctrl (Control) key is down.
        static constexpr KeyModifier RCtrl = 0x0080u; // The right Ctrl (Control) key is down.
        static constexpr KeyModifier LAlt = 0x0100u; // The left Alt key is down.
        static constexpr KeyModifier RAlt = 0x0200u; // The right Alt key is down.
        static constexpr KeyModifier LGui = 0x0400u; // The left GUI key (often the Windows key) is down.
        static constexpr KeyModifier RGui = 0x0800u; // The right GUI key (often the Windows key) is down.
        static constexpr KeyModifier Caps = 0x2000u; // The Caps Lock key is down.
        static constexpr KeyModifier Mode = 0x4000u; // The !AltGr key is down.
        static constexpr KeyModifier Scroll = 0x8000u; // The Scroll Lock key is down.

        static constexpr KeyModifier Ctrl = (LCtrl | RCtrl); // Any Ctrl key is down.
        static constexpr KeyModifier Shift = (LShift | RShift); // Any Shift key is down.
        static constexpr KeyModifier Alt = (LAlt | RAlt); // Any Alt key is down.
        static constexpr KeyModifier Gui = (LGui | RGui); // Any GUI key is down.
    };
}