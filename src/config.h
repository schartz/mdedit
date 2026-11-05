#pragma once

#include <cstdlib>
#include <filesystem>

inline std::filesystem::path configDir() {
  const char *xdg = std::getenv("XDG_CONFIG_HOME");
  std::filesystem::path base =
      (xdg && *xdg) ? std::filesystem::path(xdg)
                    : std::filesystem::path(std::getenv("HOME")) / ".config";
  return base / "mdnotes";
}

inline std::filesystem::path sessionFile() {
  return configDir() / "session.json";
}
