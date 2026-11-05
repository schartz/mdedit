#pragma once

#include "datamodel.h"
#include <filesystem>
#include <string>

std::string renderDirectoryNode(AppState &app, const std::filesystem::path &p);
void renderSidebar(AppState &app);
