#include "commands.h"

#include "documents.h"
#include "fs_utils.h"
#include "tinyfiledialogs.h"

void commandNew(AppState &app) {
  app.docs.push_back(makeNewDoc(app));
  app.active = (int)app.docs.size() - 1;
}

void commandOpen(AppState &app) {
  const char *filters[] = {"*.md", "*.markdown", "*.mdown", "*.mkd"};
  const char *sel =
      tinyfd_openFileDialog("Open Markdown File", app.sidebar_root.c_str(), 4,
                            filters, "Markdown Files", 1);
  for (auto &p : splitPaths(sel))
    openPathIntoApp(app, p);
}

void commandSave(AppState &app) {
  if (app.active < 0 || app.active >= (int)app.docs.size())
    return;
  auto &d = app.docs[app.active];
  if (d.path.empty())
    saveDocInteractive(app, d);
  else if (d.dirty)
    saveDocToPath(app, d, d.path);
}

void commandSaveAs(AppState &app) {
  if (app.active < 0 || app.active >= (int)app.docs.size())
    return;
  saveDocInteractive(app, app.docs[app.active]);
}

void commandFind(AppState &app) {
  app.find_replace.open = true;
  app.find_replace.focus_find = true;
}
