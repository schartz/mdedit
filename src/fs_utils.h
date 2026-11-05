#pragma once

#include <string>
#include <vector>

std::string readFile(const std::string &path);
bool writeFile(const std::string &path, const std::string &content);
std::string basename(const std::string &path);
std::vector<std::string> splitPaths(const char *raw);
