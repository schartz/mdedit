#include "fonts.h"
#include "icons.h"

#include <filesystem>
#include <imgui.h>
#include <iostream>

void glfwErrorCallback(int err, const char *desc) {
  std::cerr << "GLFW Error " << err << ": " << desc << "\n";
}

bool loadIconFont(float sizePx) {
  static const ImWchar iconRanges[] = {ICON_MIN, ICON_MAX, 0};
  std::string fontPath = std::string(MDNOTES_ASSETS_DIR) + "/icomoon.ttf";

  if (!std::filesystem::exists(fontPath)) {
    std::cerr << "Error: icon font not found at " << fontPath << "\n";
    return false;
  }
  ImFontConfig cfg;
  cfg.MergeMode = true;
  cfg.PixelSnapH = true;
  cfg.GlyphMinAdvanceX = 13.0f;

  if (ImGui::GetIO().Fonts->AddFontFromFileTTF(fontPath.c_str(), sizePx, &cfg,
                                               iconRanges)) {
    std::cerr << "Loaded icons from: " << fontPath << "\n";
    return true;
  }
  std::cerr << "Failed to load icon font: " << fontPath << "\n";
  return false;
}
