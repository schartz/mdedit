#include "commands.h"
#include "datamodel.h"
#include "documents.h" // <-- ADD
#include "editing_utils.h"
#include "fonts.h"
#include "fs_utils.h" // <-- ADD
#include "icons.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_rich_md/rich_md.h"
#include "imgui_stdlib.h"
#include "session.h"         // <-- ADD
#include "tinyfiledialogs.h" // <-- ADD
#include "ui/debug_utils.h"
#include "ui/document_view.h"
#include "ui/sidebar.h"
#include "ui/toolbar.h"
#include "window_icon.h"

#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <vector>

// ---------------------------------------------------------------------------
// Config / session paths
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// File helpers
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// Recent files
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// Session persistence
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// Document operations
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// Find / replace
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// Auto-save
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// Sidebar
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// Document body rendering
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// Toolbar helper
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// Set application icon
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// Font loader
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// Optional: debug grid of every codepoint in the icon font.
// Open via: View -> Debug: Icon Grid.
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// main
// ---------------------------------------------------------------------------
int main() {
  glfwSetErrorCallback(glfwErrorCallback);
  if (!glfwInit())
    return 1;

  const char *glsl_version = "#version 330";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHintString(GLFW_WAYLAND_APP_ID, "mdnotes");

  GLFWwindow *window = glfwCreateWindow(1500, 900, "MdNotes", nullptr, nullptr);
  if (!window)
    return 1;
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);
  applyWindowIcon(window, std::string(MDNOTES_ASSETS_DIR) + "/appicon.png");

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  ImGui::StyleColorsDark();

  tinyfd_messageBox("tinyfd_query", "", "", "", 0);
  std::cerr << "tinyfd backend: " << tinyfd_response << "\n";

  io.Fonts->AddFontDefault();
  loadIconFont(13.0f);

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);
  RichMd::CreateContext();

  AppState app;
  loadSession(app);
  if (app.docs.empty()) {
    app.docs.push_back(makeNewDoc(app));
    app.active = 0;
  }

  // =========================================================================
  // Main loop
  // =========================================================================
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Shortcuts
    bool doNew = ImGui::IsKeyChordPressed(ImGuiMod_Ctrl | ImGuiKey_N);
    bool doOpen = ImGui::IsKeyChordPressed(ImGuiMod_Ctrl | ImGuiKey_O);
    bool doSaveAs =
        ImGui::IsKeyChordPressed(ImGuiMod_Ctrl | ImGuiMod_Shift | ImGuiKey_S);
    bool doSave =
        !doSaveAs && ImGui::IsKeyChordPressed(ImGuiMod_Ctrl | ImGuiKey_S);
    bool doFind = ImGui::IsKeyChordPressed(ImGuiMod_Ctrl | ImGuiKey_F);
    bool open_icon_grid = false;

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(io.DisplaySize);
    ImGui::Begin("##root", nullptr,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
                     ImGuiWindowFlags_MenuBar);

    const bool hasActive =
        (app.active >= 0 && app.active < (int)app.docs.size());

    // Menu bar
    if (ImGui::BeginMenuBar()) {
      if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("New", "Ctrl+N"))
          doNew = true;
        if (ImGui::MenuItem("Open...", "Ctrl+O"))
          doOpen = true;

        if (ImGui::BeginMenu("Recent", !app.recent_files.empty())) {
          for (auto &p : app.recent_files) {
            std::string label = basename(p);
            if (ImGui::MenuItem(label.c_str()))
              openPathIntoApp(app, p);
            if (ImGui::IsItemHovered())
              ImGui::SetTooltip("%s", p.c_str());
          }
          ImGui::Separator();
          if (ImGui::MenuItem("Clear recent"))
            app.recent_files.clear();
          ImGui::EndMenu();
        }

        ImGui::Separator();
        if (ImGui::MenuItem("Save", "Ctrl+S", false, hasActive))
          doSave = true;
        if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S", false, hasActive))
          doSaveAs = true;
        ImGui::Separator();
        if (ImGui::MenuItem("Quit", "Alt+F4"))
          glfwSetWindowShouldClose(window, GLFW_TRUE);
        ImGui::EndMenu();
      }

      if (ImGui::BeginMenu("Edit")) {
        if (ImGui::MenuItem("Find / Replace", "Ctrl+F", false, hasActive))
          doFind = true;
        ImGui::EndMenu();
      }

      if (ImGui::BeginMenu("View")) {
        if (ImGui::MenuItem("Split", "Ctrl+1", app.viewMode == ViewMode::Split))
          app.viewMode = ViewMode::Split;
        if (ImGui::MenuItem("Editor only", "Ctrl+2",
                            app.viewMode == ViewMode::EditOnly))
          app.viewMode = ViewMode::EditOnly;
        if (ImGui::MenuItem("Preview only", "Ctrl+3",
                            app.viewMode == ViewMode::PreviewOnly))
          app.viewMode = ViewMode::PreviewOnly;
        ImGui::Separator();
        ImGui::MenuItem("Sidebar", nullptr, &app.sidebar_visible);
        ImGui::Separator();
        if (ImGui::MenuItem("Debug: Icon Grid"))
          open_icon_grid = true;
        ;
        ImGui::EndMenu();
      }
      ImGui::EndMenuBar();
    }

    // Handle shortcut-driven actions
    if (doNew)
      commandNew(app);
    if (doOpen)
      commandOpen(app);
    if (doSave)
      commandSave(app);
    if (doSaveAs)
      commandSaveAs(app);
    if (doFind)
      commandFind(app);

    tickAutoSave(app, ImGui::GetIO().DeltaTime);

    // Toolbar: 4 buttons (3 view modes + sidebar toggle), right-aligned
    {
      const float bw = 32.0f, gap = 4.0f;
      const float total = 4 * bw + 3 * gap;
      ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x - total);

      viewToggleButton(ICON_SPLIT_VIEW, "Split view (Ctrl+1)", app.viewMode,
                       ViewMode::Split, app.viewMode);
      ImGui::SameLine(0, gap);
      viewToggleButton(ICON_EDIT_ONLY, "Editor only (Ctrl+2)", app.viewMode,
                       ViewMode::EditOnly, app.viewMode);
      ImGui::SameLine(0, gap);
      viewToggleButton(ICON_PREVIEW_ONLY, "Preview only (Ctrl+3)", app.viewMode,
                       ViewMode::PreviewOnly, app.viewMode);
      ImGui::SameLine(0, gap);

      const bool sideActive = app.sidebar_visible;
      if (sideActive) {
        ImGui::PushStyleColor(ImGuiCol_Button,
                              ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 1));
      }
      if (ImGui::Button(ICON_SIDEBAR, ImVec2(bw, 28)))
        app.sidebar_visible = !app.sidebar_visible;
      if (sideActive)
        ImGui::PopStyleColor(2);
      if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Toggle sidebar");
    }
    ImGui::Separator();

    // Body: sidebar + tabs
    if (app.sidebar_visible) {
      renderSidebar(app);
      ImGui::SameLine();
    }

    ImGui::BeginChild("##main", ImVec2(0, 0), false);

    if (app.docs.empty()) {
      ImGui::Spacing();
      ImGui::TextDisabled(
          "No document open.  File \xe2\x86\x92 New  (Ctrl+N)  or  File "
          "\xe2\x86\x92 Open  (Ctrl+O).");
    } else {
      int closeRequested = -1;
      if (ImGui::BeginTabBar("##doc_tabs",
                             ImGuiTabBarFlags_Reorderable |
                                 ImGuiTabBarFlags_AutoSelectNewTabs)) {
        for (int i = 0; i < (int)app.docs.size(); ++i) {
          Document &d = app.docs[i];
          bool tabOpen = true;

          char label[512];
          snprintf(label, sizeof(label), "%s%s###doc%d", d.title.c_str(),
                   d.dirty ? " *" : "", d.id);

          if (ImGui::BeginTabItem(label, &tabOpen, ImGuiTabItemFlags_None)) {
            app.active = i;
            ImGui::BeginChild("##doc_body", ImVec2(0, 0), false);
            renderDocumentContent(d, app.viewMode);
            ImGui::EndChild();
            ImGui::EndTabItem();
          }
          if (!tabOpen && closeRequested < 0)
            closeRequested = i;
        }
        ImGui::EndTabBar();
      }

      if (closeRequested >= 0) {
        Document &d = app.docs[closeRequested];
        bool proceed = true;
        if (d.dirty) {
          int r = tinyfd_messageBox(
              "Unsaved changes",
              ("'" + d.title + "' has unsaved changes.\nSave before closing?")
                  .c_str(),
              "yesnocancel", "warning", 1);
          if (r == 0)
            proceed = saveDocInteractive(app, d);
          else if (r == 2)
            proceed = false;
        }
        if (proceed) {
          app.docs.erase(app.docs.begin() + closeRequested);
          if (app.active >= (int)app.docs.size())
            app.active = (int)app.docs.size() - 1;
        }
      }
    }

    ImGui::EndChild();

    // Find / Replace modal
    if (app.find_replace.open)
      ImGui::OpenPopup("Find & Replace");
    if (ImGui::BeginPopupModal("Find & Replace", nullptr,
                               ImGuiWindowFlags_AlwaysAutoResize)) {
      auto &fr = app.find_replace;

      ImGui::Text("Find:");
      ImGui::SameLine(90);
      if (fr.focus_find) {
        ImGui::SetKeyboardFocusHere();
        fr.focus_find = false;
      }
      ImGui::InputText("##find", &fr.find_term);

      ImGui::Text("Replace:");
      ImGui::SameLine(90);
      ImGui::InputText("##replace", &fr.replace_term);

      ImGui::Spacing();
      if (ImGui::Button("Find Next"))
        findNext(app);
      ImGui::SameLine();
      if (ImGui::Button("Replace"))
        replaceCurrent(app);
      ImGui::SameLine();
      if (ImGui::Button("Replace All"))
        replaceAll(app);

      ImGui::Spacing();
      if (!fr.status.empty())
        ImGui::TextDisabled("%s", fr.status.c_str());

      ImGui::Separator();
      if (ImGui::Button("Close", ImVec2(120, 0))) {
        fr.open = false;
        ImGui::CloseCurrentPopup();
      }
      ImGui::EndPopup();
    }
    if (open_icon_grid)
      ImGui::OpenPopup("Icon Grid");

    renderIconDebugPopup();
    ImGui::End();

    // Render
    ImGui::Render();
    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    glViewport(0, 0, w, h);
    glClearColor(0.10f, 0.10f, 0.10f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
  }

  saveSession(app);

  RichMd::DestroyContext();
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
