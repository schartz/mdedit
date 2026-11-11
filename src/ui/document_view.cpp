#include "ui/document_view.h"
#include "fonts.h"
#include "imgui.h"
#include "imgui_rich_md/rich_md.h"
#include "imgui_stdlib.h"
#include "ui/theme_colors.h"

// ---------------------------------------------------------------------------
// Monokai — editor
// ---------------------------------------------------------------------------
static void pushMonokaiEditorColors() {
  ImGui::PushStyleColor(ImGuiCol_FrameBg, ThemeColors::Mono_Bg);
  ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ThemeColors::Mono_BgHover);
  ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ThemeColors::Mono_BgHover);
  ImGui::PushStyleColor(ImGuiCol_Text, ThemeColors::Mono_Text);
  ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, ThemeColors::Mono_TextSel);
  ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, ThemeColors::Mono_Bg);
  ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, ThemeColors::Mono_BgHover);
  ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabHovered,
                        ThemeColors::Mono_TextSel);
  ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabActive,
                        ThemeColors::Mono_TextSel);
  ImGui::PushStyleColor(ImGuiCol_Border, ThemeColors::Mono_Border);
  ImGui::PushStyleColor(ImGuiCol_Separator, ThemeColors::Mono_Border);
}

static void popMonokaiEditorColors() { ImGui::PopStyleColor(11); }

// ---------------------------------------------------------------------------
// Sublime-neutral dark — rendered markdown preview
// ---------------------------------------------------------------------------
static void pushAtomPreviewColors() {
  ImGui::PushStyleColor(ImGuiCol_ChildBg, ThemeColors::Atom_Bg);
  ImGui::PushStyleColor(ImGuiCol_Text, ThemeColors::Atom_Text);
  ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, ThemeColors::Atom_TextSel);
  ImGui::PushStyleColor(ImGuiCol_FrameBg, ThemeColors::Atom_BgAlt);
  ImGui::PushStyleColor(ImGuiCol_Border, ThemeColors::Atom_Border);
  ImGui::PushStyleColor(ImGuiCol_Separator, ThemeColors::Atom_Border);
#if defined(ImGuiCol_TextLink)
  ImGui::PushStyleColor(ImGuiCol_TextLink, ThemeColors::Atom_Link);
#endif
}

static void popAtomPreviewColors() {
#if defined(ImGuiCol_TextLink)
  ImGui::PopStyleColor(7);
#else
  ImGui::PopStyleColor(6);
#endif
}

// ---------------------------------------------------------------------------
// renderDocumentContent
// ---------------------------------------------------------------------------
void renderDocumentContent(Document &d, ViewMode mode, int contentFontIdx) {
  bool changed = false;
  ImFont *contentFont = getContentFont(contentFontIdx);

  auto renderEditor = [&]() {
    ImGui::PushID("editor");

    // Full-bleed Monokai background behind the editor widget.
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ThemeColors::Mono_Bg);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::BeginChild("##editor_bg", ImVec2(0, 0), false);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8.0f, 6.0f));

    if (contentFont)
      ImGui::PushFont(contentFont);

    pushMonokaiEditorColors();
    if (ImGui::InputTextMultiline("##text", &d.content,
                                  ImVec2(-FLT_MIN, -FLT_MIN)))
      changed = true;
    popMonokaiEditorColors();

    if (contentFont)
      ImGui::PopFont();

    ImGui::PopStyleVar(2); // FramePadding, WindowPadding
    ImGui::EndChild();
    ImGui::PopStyleColor(); // ChildBg
    ImGui::PopID();
  };

  auto renderPreview = [&]() {
    ImGui::PushID("preview");

    pushAtomPreviewColors();
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16.0f, 12.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(6.0f, 8.0f));

    ImGui::BeginChild("##preview_bg", ImVec2(0, 0), false);

    if (contentFont)
      ImGui::PushFont(contentFont);

    RichMd::Render(d.content);

    if (contentFont)
      ImGui::PopFont();

    ImGui::EndChild();

    ImGui::PopStyleVar(2);
    popAtomPreviewColors();
    ImGui::PopID();
  };

  switch (mode) {
  case ViewMode::EditOnly:
    renderEditor();
    break;
  case ViewMode::PreviewOnly:
    renderPreview();
    break;
  case ViewMode::Split: {
    ImGui::Columns(2, "##split", true);
    renderEditor();
    ImGui::NextColumn();
    renderPreview();
    ImGui::Columns(1);
    break;
  }
  }

  if (changed)
    d.dirty = true;
}
