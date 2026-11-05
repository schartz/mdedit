#include "ui/sidebar.h"
#include "documents.h"
#include "icons.h"
#include "imgui.h"
#include "tinyfiledialogs.h"
#include <algorithm>

std::string renderDirectoryNode(AppState &app, const std::filesystem::path &p) {
  std::string clicked;
  std::error_code ec;

  std::vector<std::filesystem::directory_entry> entries;
  for (auto &e : std::filesystem::directory_iterator(
           p, std::filesystem::directory_options::skip_permission_denied, ec))
    entries.push_back(e);

  std::sort(entries.begin(), entries.end(), [](auto &a, auto &b) {
    bool da = a.is_directory(), db = b.is_directory();
    if (da != db)
      return da > db;
    return a.path().filename().string() < b.path().filename().string();
  });

  for (auto &e : entries) {
    const std::filesystem::path &ep = e.path();
    std::string name = ep.filename().string();
    if (name.empty() || name[0] == '.')
      continue;
    bool isDir = e.is_directory(ec);

    // PushID ensures each node gets a unique ID even though
    // std::filesystem::path strings are stored in temporaries whose address may
    // be reused across iterations.
    ImGui::PushID(ep.string().c_str());

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow |
                               ImGuiTreeNodeFlags_SpanAvailWidth |
                               ImGuiTreeNodeFlags_DefaultOpen;
    if (!isDir) {
      flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
      if (app.active >= 0 && app.docs[app.active].path == ep.string())
        flags |= ImGuiTreeNodeFlags_Selected;
    }

    std::string label =
        std::string(isDir ? ICON_FOLDER_OPEN " " : ICON_FILE " ") + name;
    bool open = ImGui::TreeNodeEx("node", flags, "%s", label.c_str());

    if (ImGui::IsItemClicked() && !isDir)
      clicked = ep.string();
    if (ImGui::IsItemHovered())
      ImGui::SetTooltip("%s", ep.string().c_str());

    if (isDir && open) {
      std::string sub = renderDirectoryNode(app, ep);
      if (!sub.empty())
        clicked = sub;
      ImGui::TreePop();
    }

    ImGui::PopID();
  }
  return clicked;
}

void renderSidebar(AppState &app) {
  if (!app.sidebar_visible)
    return;

  ImGui::BeginChild("##sidebar", ImVec2(260, 0), true);

  ImGui::TextDisabled("EXPLORER");
  ImGui::SameLine(ImGui::GetContentRegionMax().x - 30);
  if (ImGui::SmallButton(ICON_FOLDER_OPEN)) {
    const char *p = tinyfd_selectFolderDialog("Choose notes folder",
                                              app.sidebar_root.c_str());
    if (p)
      app.sidebar_root = p;
  }
  if (ImGui::IsItemHovered())
    ImGui::SetTooltip("Open folder...");

  ImGui::Separator();

  if (app.sidebar_root.empty()) {
    ImGui::TextDisabled("No folder opened.");
    ImGui::TextWrapped(
        "Click the folder icon above to choose a notes directory.");
  } else {
    ImGui::TextWrapped("%s", app.sidebar_root.c_str());
    ImGui::Separator();
    std::string clicked = renderDirectoryNode(app, app.sidebar_root);
    if (!clicked.empty())
      openPathIntoApp(app, clicked);
  }

  ImGui::EndChild();
}
