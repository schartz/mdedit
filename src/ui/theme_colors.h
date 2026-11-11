#pragma once

#include "imgui.h"

namespace ThemeColors {

// ---- Monokai --------------------------------------------------------------
// Classic Monokai palette (monokai.pro / Sublime defaults).
inline const ImVec4 Mono_Bg =
    ImVec4(0.1529f, 0.1569f, 0.1333f, 1.0f); // #272822
inline const ImVec4 Mono_BgHover =
    ImVec4(0.2431f, 0.2392f, 0.1961f, 1.0f); // #3e3d32
inline const ImVec4 Mono_Text =
    ImVec4(0.9725f, 0.9725f, 0.9490f, 1.0f); // #f8f8f2
inline const ImVec4 Mono_TextSel =
    ImVec4(0.2863f, 0.2824f, 0.2431f, 1.0f); // #49483e
inline const ImVec4 Mono_Border =
    ImVec4(0.2902f, 0.2863f, 0.2471f, 1.0f); // #4a493f

// ---- Preview panel — Sublime-neutral dark (matches chrome, separates from
//      the olive Monokai editor) ---------------------------------------------
inline const ImVec4 Atom_Bg = ImVec4(0.129f, 0.133f, 0.137f, 1.0f); // #212223
inline const ImVec4 Atom_BgAlt =
    ImVec4(0.102f, 0.106f, 0.110f, 1.0f);                             // #1a1b1c
inline const ImVec4 Atom_Text = ImVec4(0.788f, 0.792f, 0.788f, 1.0f); // #c9cac9
inline const ImVec4 Atom_TextDim =
    ImVec4(0.463f, 0.467f, 0.463f, 1.0f); // #767776
inline const ImVec4 Atom_TextSel =
    ImVec4(0.243f, 0.247f, 0.251f, 1.0f); // #3e3f40
inline const ImVec4 Atom_Link = ImVec4(
    0.400f, 0.851f, 0.937f, 1.0f); // #66D9EF (Monokai cyan, ties to editor)
inline const ImVec4 Atom_Border =
    ImVec4(0.169f, 0.173f, 0.176f, 1.0f); // #2b2c2d

} // namespace ThemeColors
