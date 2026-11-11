#include "ui/theme.h"
#include "imgui.h"

void applyAppTheme() {
  ImGuiStyle &style = ImGui::GetStyle();
  ImVec4 *c = style.Colors;

  // ---- Spacing / rounding (Sublime: minimal, mostly flat) ----------------
  style.WindowPadding = ImVec2(8, 8);
  style.FramePadding = ImVec2(8, 4);
  style.CellPadding = ImVec2(4, 2);
  style.ItemSpacing = ImVec2(8, 6);
  style.ItemInnerSpacing = ImVec2(6, 4);
  style.IndentSpacing = 20.0f;
  style.ScrollbarSize = 12.0f;
  style.GrabMinSize = 10.0f;

  style.WindowRounding = 4.0f;
  style.ChildRounding = 2.0f;
  style.FrameRounding = 2.0f;
  style.PopupRounding = 4.0f;
  style.ScrollbarRounding = 4.0f;
  style.GrabRounding = 2.0f;
  style.TabRounding = 0.0f;

  style.WindowBorderSize = 0.0f;
  style.ChildBorderSize = 1.0f;
  style.PopupBorderSize = 1.0f;
  style.FrameBorderSize = 0.0f;
  style.TabBorderSize = 1.0f;

  style.WindowMenuButtonPosition = ImGuiDir_None;
  style.AntiAliasedLines = true;
  style.AntiAliasedLinesUseTex = true;
  style.AntiAliasedFill = true;

  // ---- Sublime Text "Adaptive (dark)" palette ----------------------------
  // Neutral dark grays with the signature warm-orange accent.
  const ImVec4 bg = ImVec4(0.129f, 0.133f, 0.137f, 1.00f);        // #212223
  const ImVec4 bgDark = ImVec4(0.102f, 0.106f, 0.110f, 1.00f);    // #1a1b1c
  const ImVec4 bgMid = ImVec4(0.161f, 0.165f, 0.169f, 1.00f);     // #292a2b
  const ImVec4 bgLight = ImVec4(0.204f, 0.208f, 0.212f, 1.00f);   // #343536
  const ImVec4 bgLighter = ImVec4(0.243f, 0.247f, 0.251f, 1.00f); // #3e3f40
  const ImVec4 text = ImVec4(0.788f, 0.792f, 0.788f, 1.00f);      // #c9cac9
  const ImVec4 textDim = ImVec4(0.463f, 0.467f, 0.463f, 1.00f);   // #767776
  const ImVec4 border = ImVec4(0.169f, 0.173f, 0.176f, 1.00f);    // #2b2c2d
  const ImVec4 accent = ImVec4(1.000f, 0.624f, 0.110f, 1.00f);    // #FF9F1C
  const ImVec4 accentDim = ImVec4(1.000f, 0.624f, 0.110f, 0.35f);
  const ImVec4 accentBg = ImVec4(1.000f, 0.624f, 0.110f, 0.15f);

  // ---- Base --------------------------------------------------------------
  c[ImGuiCol_Text] = text;
  c[ImGuiCol_TextDisabled] = textDim;
  c[ImGuiCol_WindowBg] = bg;
  c[ImGuiCol_ChildBg] = ImVec4(0, 0, 0, 0);
  c[ImGuiCol_PopupBg] = bgDark;
  c[ImGuiCol_Border] = border;
  c[ImGuiCol_BorderShadow] = ImVec4(0, 0, 0, 0);

  // ---- Frame -------------------------------------------------------------
  c[ImGuiCol_FrameBg] = bgMid;
  c[ImGuiCol_FrameBgHovered] = bgLight;
  c[ImGuiCol_FrameBgActive] = bgLighter;

  // ---- Title / menu bar --------------------------------------------------
  c[ImGuiCol_TitleBg] = bgDark;
  c[ImGuiCol_TitleBgActive] = bgDark;
  c[ImGuiCol_TitleBgCollapsed] = bgDark;
  c[ImGuiCol_MenuBarBg] = bgDark;

  // ---- Scrollbar ---------------------------------------------------------
  c[ImGuiCol_ScrollbarBg] = bg;
  c[ImGuiCol_ScrollbarGrab] = bgMid;
  c[ImGuiCol_ScrollbarGrabHovered] = bgLight;
  c[ImGuiCol_ScrollbarGrabActive] = accent;

  // ---- Interactive -------------------------------------------------------
  c[ImGuiCol_CheckMark] = accent;
  c[ImGuiCol_SliderGrab] = accent;
  c[ImGuiCol_SliderGrabActive] = accent;

  c[ImGuiCol_Button] = bgMid;
  c[ImGuiCol_ButtonHovered] = bgLight;
  c[ImGuiCol_ButtonActive] = accent; // used as the "toggle is on" state

  c[ImGuiCol_Header] = bgMid;
  c[ImGuiCol_HeaderHovered] = bgLight;
  c[ImGuiCol_HeaderActive] = bgLighter;

  c[ImGuiCol_Separator] = border;
  c[ImGuiCol_SeparatorHovered] = accentDim;
  c[ImGuiCol_SeparatorActive] = accent;

  c[ImGuiCol_ResizeGrip] = ImVec4(0, 0, 0, 0);
  c[ImGuiCol_ResizeGripHovered] = accentDim;
  c[ImGuiCol_ResizeGripActive] = accent;

  // ---- Tabs ---------------------------------------------------------------
  // Sublime-flat:
  //   inactive tab  = preview background (#212223 neutral dark)
  //   active tab    = editor background (Monokai #272822)
  //   hovered       = one shade up from inactive
  //   no overline   — the color jump does the identifying work.
  c[ImGuiCol_Tab] = ImVec4(0.129f, 0.133f, 0.137f, 1.00f);        // preview bg
  c[ImGuiCol_TabHovered] = ImVec4(0.165f, 0.169f, 0.173f, 1.00f); // #2a2b2c
#if defined(ImGuiCol_TabSelected)
  c[ImGuiCol_TabSelected] = ImVec4(0.153f, 0.157f, 0.133f, 1.00f); // Monokai
  c[ImGuiCol_TabSelectedOverline] =
      ImVec4(0.153f, 0.157f, 0.133f, 1.00f); // hidden
  c[ImGuiCol_TabDimmed] = ImVec4(0.129f, 0.133f, 0.137f, 1.00f);
  c[ImGuiCol_TabDimmedSelected] = ImVec4(0.153f, 0.157f, 0.133f, 1.00f);
#if defined(ImGuiCol_TabDimmedSelectedOverline)
  c[ImGuiCol_TabDimmedSelectedOverline] = ImVec4(0.153f, 0.157f, 0.133f, 1.00f);
#endif
#else
  c[ImGuiCol_TabActive] = ImVec4(0.153f, 0.157f, 0.133f, 1.00f);
  c[ImGuiCol_TabUnfocused] = ImVec4(0.129f, 0.133f, 0.137f, 1.00f);
  c[ImGuiCol_TabUnfocusedActive] = ImVec4(0.153f, 0.157f, 0.133f, 1.00f);
#endif
#if defined(ImGuiCol_TabBarBg)
  c[ImGuiCol_TabBarBg] = ImVec4(0.129f, 0.133f, 0.137f, 1.00f); // preview bg
#endif

  // ---- Plot / tables -----------------------------------------------------
  c[ImGuiCol_PlotLines] = textDim;
  c[ImGuiCol_PlotLinesHovered] = accent;
  c[ImGuiCol_PlotHistogram] = accent;
  c[ImGuiCol_PlotHistogramHovered] = accent;

  c[ImGuiCol_TableHeaderBg] = bgDark;
  c[ImGuiCol_TableBorderStrong] = border;
  c[ImGuiCol_TableBorderLight] = bgMid;
  c[ImGuiCol_TableRowBg] = ImVec4(0, 0, 0, 0);
  c[ImGuiCol_TableRowBgAlt] = ImVec4(1, 1, 1, 0.02f);

  // ---- Selection / misc --------------------------------------------------
  c[ImGuiCol_TextSelectedBg] = accentBg;
  c[ImGuiCol_DragDropTarget] = accent;
#if defined(ImGuiCol_NavCursor)
  c[ImGuiCol_NavCursor] = accent;
#else
  c[ImGuiCol_NavHighlight] = accent;
#endif
  c[ImGuiCol_NavWindowingHighlight] = ImVec4(1, 1, 1, 0.7f);
  c[ImGuiCol_NavWindowingDimBg] = ImVec4(0, 0, 0, 0.55f);
  c[ImGuiCol_ModalWindowDimBg] = ImVec4(0, 0, 0, 0.55f);
#if defined(ImGuiCol_TextLink)
  c[ImGuiCol_TextLink] = accent;
#endif
}
