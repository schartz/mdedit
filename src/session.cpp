
#include "session.h"
#include "config.h"
#include "fs_utils.h"

#include <fstream>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

void saveSession(const AppState &app) {
  nlohmann::json j;
  j["recent_files"] = app.recent_files;
  j["sidebar_root"] = app.sidebar_root;
  j["view_mode"] = (int)app.viewMode;
  j["content_font_idx"] = app.content_font_idx;

  nlohmann::json docsJson = nlohmann::json::array();
  for (const auto &d : app.docs) {
    if (d.path.empty() && !d.dirty)
      continue;
    docsJson.push_back({{"path", d.path},
                        {"title", d.title},
                        {"content", d.dirty ? d.content : ""},
                        {"dirty", d.dirty}});
  }
  j["open_docs"] = docsJson;

  std::error_code ec;
  std::filesystem::create_directories(configDir(), ec);
  std::ofstream f(sessionFile());
  if (f)
    f << j.dump(2);
}

void loadSession(AppState &app) {
  std::ifstream f(sessionFile());
  if (!f)
    return;
  nlohmann::json j;
  try {
    f >> j;
  } catch (...) {
    return;
  }

  if (j.contains("content_font_idx"))
    app.content_font_idx = j["content_font_idx"].get<int>();
  if (j.contains("recent_files"))
    app.recent_files = j["recent_files"].get<std::vector<std::string>>();
  if (j.contains("sidebar_root"))
    app.sidebar_root = j["sidebar_root"].get<std::string>();
  if (j.contains("view_mode"))
    app.viewMode = (ViewMode)j["view_mode"].get<int>();

  if (j.contains("open_docs")) {
    for (auto &entry : j["open_docs"]) {
      Document d;
      d.id = app.nextDocId++;
      d.path = entry.value("path", "");
      d.title = entry.value("title", "Untitled");
      d.dirty = entry.value("dirty", false);

      if (d.dirty) {
        d.content = entry.value("content", "");
      } else if (!d.path.empty() && std::filesystem::exists(d.path)) {
        d.content = readFile(d.path);
      } else {
        continue;
      }
      app.docs.push_back(std::move(d));
    }
    if (!app.docs.empty())
      app.active = 0;
  }
}
