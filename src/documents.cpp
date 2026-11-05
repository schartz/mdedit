#include "documents.h"

#include "fs_utils.h"
#include "tinyfiledialogs.h"

#include <algorithm>
#include <filesystem>
#include <string>

static void addRecentFile(AppState &app, const std::string &path) {
  if (path.empty())
    return;
  auto &v = app.recent_files;
  v.erase(std::remove(v.begin(), v.end(), path), v.end());
  v.insert(v.begin(), path);
  if (v.size() > 10)
    v.resize(10);
}

Document makeNewDoc(AppState &app) {
  Document d;
  d.id = app.nextDocId++;
  d.title = "Untitled " + std::to_string(app.untitledCounter++);
  d.content = "# New Note\n\nStart typing...\n";
  return d;
}

void openPathIntoApp(AppState &app, const std::string &path) {
  for (size_t i = 0; i < app.docs.size(); ++i) {
    if (app.docs[i].path == path) {
      app.active = (int)i;
      return;
    }
  }
  Document d;
  d.id = app.nextDocId++;
  d.path = path;
  d.title = basename(path);
  d.content = readFile(path);
  app.docs.push_back(std::move(d));
  app.active = (int)app.docs.size() - 1;
  addRecentFile(app, path);
}

bool saveDocToPath(AppState &app, Document &d, const std::string &path) {
  if (!writeFile(path, d.content)) {
    tinyfd_messageBox("Save failed", ("Could not write to:\n" + path).c_str(),
                      "ok", "error", 1);
    return false;
  }
  d.path = path;
  d.title = basename(path);
  d.dirty = false;
  addRecentFile(app, path);
  return true;
}

bool saveDocInteractive(AppState &app, Document &d) {
  if (!d.path.empty())
    return saveDocToPath(app, d, d.path);

  const char *filters[] = {"*.md", "*.markdown", "*.mdown"};
  const char *path = tinyfd_saveFileDialog("Save File As", d.title.c_str(), 3,
                                           filters, "Markdown Files");
  if (!path)
    return false;
  std::string p = path;
  if (std::filesystem::path(p).extension().empty())
    p += ".md";
  return saveDocToPath(app, d, p);
}

void tickAutoSave(AppState &app, float deltaTime) {
  if (app.autosave_interval <= 0.0f)
    return;
  app.autosave_timer += deltaTime;
  if (app.autosave_timer < app.autosave_interval)
    return;
  app.autosave_timer = 0.0f;
  for (auto &d : app.docs)
    if (d.dirty && !d.path.empty())
      saveDocToPath(app, d, d.path);
}
