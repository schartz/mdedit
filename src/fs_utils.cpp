#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

std::string readFile(const std::string &path) {
  std::ifstream f(path);
  if (!f)
    return {};
  std::stringstream ss;
  ss << f.rdbuf();
  return ss.str();
}

bool writeFile(const std::string &path, const std::string &content) {
  std::ofstream f(path);
  if (!f)
    return false;
  f << content;
  return true;
}

std::string basename(const std::string &path) {
  return std::filesystem::path(path).filename().string();
}

std::vector<std::string> splitPaths(const char *raw) {
  std::vector<std::string> out;
  if (!raw || !*raw)
    return out;
  std::string s(raw);
  size_t start = 0, pos;
  while ((pos = s.find('|', start)) != std::string::npos) {
    out.push_back(s.substr(start, pos - start));
    start = pos + 1;
  }
  out.push_back(s.substr(start));
  return out;
}
