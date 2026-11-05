#include "ui/toolbar.h"
#include "imgui.h"
void viewToggleButton(const char *icon, const char *tip, ViewMode current,
                      ViewMode target, ViewMode &out) {
  bool isActive = (current == target);
  if (isActive) {
    ImGui::PushStyleColor(ImGuiCol_Button,
                          ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 1));
  }
  if (ImGui::Button(icon, ImVec2(32, 28)))
    out = target;
  if (isActive)
    ImGui::PopStyleColor(2);
  if (ImGui::IsItemHovered())
    ImGui::SetTooltip("%s", tip);
}
