#include "ui/sidebar.h"
#include "documents.h"
#include "icons.h"
#include "imgui.h"
#include "tinyfiledialogs.h"
#include "ui/theme.h"
#include <algorithm>
#include <filesystem>
#include <string>
#include <utility>
#include <vector>

// ---------------------------------------------------------------------------
// Directory tree cache
//
// The sidebar used to walk (and sort) the whole notes directory recursively on
// every frame. That is 60 scans/second doing hundreds of stat() calls and
// string allocations for a tree that almost never changes. We now build the
// tree once per "generation" and render from that snapshot; a refresh button
// (and opening / saving a file) invalidates it.
// ---------------------------------------------------------------------------

namespace {

struct SidebarNode {
  std::string name;
  std::string path;
  bool is_dir = false;
  std::vector<SidebarNode> children;
};

struct SidebarTree {
  std::string root;
  SidebarNode root_node;
  bool valid = false;
};

SidebarTree g_tree;

constexpr int kMaxTreeDepth = 64; // guards against pathological symlink loops

void scanDirectory(const std::filesystem::path &dir,
                   std::vector<SidebarNode> &out, int depth) {
  struct Entry {
    std::string name;
    std::filesystem::path path;
    bool is_dir = false;
  };

  std::vector<Entry> entries;
  std::error_code ec;
  std::filesystem::directory_iterator it(
      dir, std::filesystem::directory_options::skip_permission_denied, ec);
  const std::filesystem::directory_iterator end;
  while (!ec && it != end) {
    const std::filesystem::directory_entry &e = *it;
    Entry entry;
    entry.path = e.path();
    entry.name = entry.path.filename().string();
    std::error_code entry_ec;
    // Hidden files are skipped, as before.
    if (!entry.name.empty() && entry.name[0] != '.') {
      const bool is_symlink = e.is_symlink(entry_ec);
      entry.is_dir = e.is_directory(entry_ec);
      // Do not recurse through symlinks: a link pointing back up the tree
      // would make the scan loop forever.
      entry.is_dir = entry.is_dir && !is_symlink;
      entries.push_back(std::move(entry));
    }
    it.increment(ec); // error_code overload: never throws
  }

  std::sort(entries.begin(), entries.end(), [](const Entry &a, const Entry &b) {
    if (a.is_dir != b.is_dir)
      return a.is_dir > b.is_dir;
    return a.name < b.name;
  });

  out.reserve(entries.size());
  for (Entry &e : entries) {
    SidebarNode node;
    node.name = std::move(e.name);
    node.path = e.path.string();
    node.is_dir = e.is_dir;
    if (node.is_dir && depth < kMaxTreeDepth)
      scanDirectory(e.path, node.children, depth + 1);
    out.push_back(std::move(node));
  }
}

void ensureTreeBuilt(const AppState &app) {
  if (g_tree.valid && g_tree.root == app.sidebar_root)
    return;

  g_tree.root = app.sidebar_root;
  g_tree.root_node = SidebarNode{};
  g_tree.root_node.path = app.sidebar_root;
  g_tree.root_node.is_dir = true;
  g_tree.valid = true;

  if (!app.sidebar_root.empty())
    scanDirectory(app.sidebar_root, g_tree.root_node.children, 0);
}

void renderNodes(const AppState &app, const std::vector<SidebarNode> &nodes,
                 std::string &clicked) {
  for (const SidebarNode &node : nodes) {
    // PushID ensures each node gets a unique ID even though the strings are
    // stored in temporaries whose address may be reused across iterations.
    ImGui::PushID(node.path.c_str());

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow |
                               ImGuiTreeNodeFlags_SpanAvailWidth |
                               ImGuiTreeNodeFlags_DefaultOpen;
    if (!node.is_dir) {
      flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
      if (app.active >= 0 && app.active < (int)app.docs.size() &&
          app.docs[app.active].path == node.path)
        flags |= ImGuiTreeNodeFlags_Selected;
    }

    std::string label =
        std::string(node.is_dir ? ICON_FOLDER_OPEN " " : ICON_FILE " ") +
        node.name;
    bool open = ImGui::TreeNodeEx("node", flags, "%s", label.c_str());

    if (ImGui::IsItemClicked() && !node.is_dir)
      clicked = node.path;
    if (ImGui::IsItemHovered())
      ImGui::SetTooltip("%s", node.path.c_str());

    if (node.is_dir && open) {
      renderNodes(app, node.children, clicked);
      ImGui::TreePop();
    }

    ImGui::PopID();
  }
}

} // namespace

void invalidateSidebarCache() { g_tree.valid = false; }

void renderSidebar(AppState &app) {
  if (!app.sidebar_visible)
    return;

  ensureTreeBuilt(app);

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
  ImGui::BeginChild("##sidebar", ImVec2(260, 0), true);

  // ---- EXPLORER header — same total height as the tab bar ----------------
  {
    const float textH = ImGui::GetTextLineHeight();
    const float headerH = textH + 2.0f * kTabFramePaddingY;
    const float btnSz = textH + 4.0f;           // icon buttons
    const float textY = (headerH - textH) * 0.5f;
    const float btnY = (headerH - btnSz) * 0.5f;
    const float leftPad = 8.0f;
    const float rightPad = 6.0f;

    ImGui::PushStyleColor(ImGuiCol_ChildBg,
                          ImGui::GetStyleColorVec4(ImGuiCol_MenuBarBg));
    // Child with zero internal padding — we position children explicitly so
    // text and buttons are vertically centered regardless of font / DPI.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::BeginChild("##explorer_header", ImVec2(0, headerH), false,
                      ImGuiWindowFlags_NoScrollbar |
                          ImGuiWindowFlags_NoScrollWithMouse);

    // "EXPLORER" label — vertically centered
    ImGui::SetCursorPos(ImVec2(leftPad, textY));
    ImGui::TextDisabled("EXPLORER");

    // Right-aligned: [Refresh] [folder]
    const float folderX =
        ImGui::GetContentRegionMax().x - btnSz - rightPad;
    const char *refreshLabel = "Refresh";
    const float refreshW =
        ImGui::CalcTextSize(refreshLabel).x +
        2.0f * ImGui::GetStyle().FramePadding.x;
    const float refreshX = folderX - refreshW - 6.0f;

    ImGui::SetCursorPos(ImVec2(refreshX, btnY));
    if (ImGui::Button(refreshLabel, ImVec2(refreshW, btnSz)))
      invalidateSidebarCache();
    if (ImGui::IsItemHovered())
      ImGui::SetTooltip("Re-scan the notes folder");

    ImGui::SetCursorPos(ImVec2(folderX, btnY));
    if (ImGui::Button(ICON_FOLDER_OPEN, ImVec2(btnSz, btnSz))) {
      const char *p = tinyfd_selectFolderDialog("Choose notes folder",
                                                app.sidebar_root.c_str());
      if (p)
        app.sidebar_root = p;
    }
    if (ImGui::IsItemHovered())
      ImGui::SetTooltip("Open folder...");

    ImGui::EndChild();
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
  }

  ImGui::Separator();

  // ---- Body --------------------------------------------------------------
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
  ImGui::BeginChild("##sidebar_body", ImVec2(0, 0), false);

  if (app.sidebar_root.empty()) {
    ImGui::TextDisabled("No folder opened.");
    ImGui::TextWrapped(
        "Click the folder icon above to choose a notes directory.");
  } else {
    ImGui::TextWrapped("%s", app.sidebar_root.c_str());
    ImGui::Separator();
    std::string clicked;
    renderNodes(app, g_tree.root_node.children, clicked);
    if (!clicked.empty())
      openPathIntoApp(app, clicked);
  }

  ImGui::EndChild();
  ImGui::PopStyleVar();

  ImGui::EndChild();
  ImGui::PopStyleVar();
}
