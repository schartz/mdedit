#pragma once

#include "datamodel.h"

Document makeNewDoc(AppState &app);
void openPathIntoApp(AppState &app, const std::string &path);
bool saveDocToPath(AppState &app, Document &d, const std::string &path);
bool saveDocInteractive(AppState &app, Document &d);
void tickAutoSave(AppState &app, float deltaTime);
