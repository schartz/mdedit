#include "ui/document_view.h"
#include "imgui.h"
#include "imgui_rich_md/rich_md.h"
#include "imgui_stdlib.h"

void renderDocumentContent(Document &d, ViewMode mode) {
  bool changed = false;

  auto renderEditor = [&]() {
    ImGui::PushID("editor");
    if (ImGui::InputTextMultiline("##text", &d.content,
                                  ImVec2(-FLT_MIN, -FLT_MIN)))
      changed = true;
    ImGui::PopID();
  };
  auto renderPreview = [&]() {
    ImGui::PushID("preview");
    ImGui::BeginChild("##child", ImVec2(0, 0), false);
    RichMd::Render(d.content);
    ImGui::EndChild();
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
