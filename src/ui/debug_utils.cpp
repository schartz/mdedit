#include "ui/debug_utils.h"
#include "icons.h"
#include "imgui.h"
void renderIconDebugPopup() {
  if (!ImGui::BeginPopupModal("Icon Grid", nullptr,
                              ImGuiWindowFlags_AlwaysAutoResize))
    return;

  ImGui::Text("Glyphs from icomoon.ttf in range U+%04X..U+%04X", ICON_MIN,
              ICON_MAX);
  ImGui::Separator();

  const int cols = 8;
  int col = 0;
  for (int cp = ICON_MIN; cp <= ICON_MAX; ++cp) {
    char utf8[5] = {0};
    if (cp < 0x800) {
      utf8[0] = (char)(0xC0 | (cp >> 6));
      utf8[1] = (char)(0x80 | (cp & 0x3F));
    } else {
      utf8[0] = (char)(0xE0 | (cp >> 12));
      utf8[1] = (char)(0x80 | ((cp >> 6) & 0x3F));
      utf8[2] = (char)(0x80 | (cp & 0x3F));
    }
    ImGui::BeginGroup();
    ImGui::Button(utf8, ImVec2(40, 40));
    ImGui::TextDisabled("%04X", cp);
    ImGui::EndGroup();
    if (++col % cols != 0)
      ImGui::SameLine();
  }

  ImGui::NewLine();
  ImGui::Separator();
  if (ImGui::Button("Close", ImVec2(120, 0)))
    ImGui::CloseCurrentPopup();
  ImGui::EndPopup();
}
