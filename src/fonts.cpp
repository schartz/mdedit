#include "fonts.h"
#include "icons.h"

#include <filesystem>
#include <imgui.h>
#include <iostream>

void glfwErrorCallback(int err, const char *desc) {
  std::cerr << "GLFW Error " << err << ": " << desc << "\n";
}

bool loadIconFont(float sizePx, float dpiScale) {
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
  // Rasterize at the real DPI, like the text fonts. The app computes ImGui
  // frames at density 1 (see the workaround in main.cpp), so the font config
  // is what keeps icon glyphs crisp on HiDPI displays.
  cfg.RasterizerDensity = dpiScale;

  if (ImGui::GetIO().Fonts->AddFontFromFileTTF(fontPath.c_str(), sizePx, &cfg,
                                               iconRanges)) {
    std::cerr << "Loaded icons from: " << fontPath << "\n";
    return true;
  }
  std::cerr << "Failed to load icon font: " << fontPath << "\n";
  return false;
}

bool loadMainFont(float sizePx, float dpiScale) {
  ImGuiIO &io = ImGui::GetIO();

  std::string fontPath = std::string(IMGUI_FONTS_DIR) + "/Cousine-Regular.ttf";

  if (!std::filesystem::exists(fontPath)) {
    std::cerr << "Error: main font not found at " << fontPath
              << "; falling back to default.\n";
    io.Fonts->AddFontDefault();
    return false;
  }

  ImFontConfig cfg;
  cfg.OversampleH = 2;
  cfg.OversampleV = 2;
  cfg.PixelSnapH = false;
  cfg.RasterizerDensity = dpiScale;
  cfg.FontDataOwnedByAtlas = true;

  ImFont *font = io.Fonts->AddFontFromFileTTF(fontPath.c_str(), sizePx, &cfg);
  if (!font) {
    std::cerr << "Failed to load main font: " << fontPath << "\n";
    io.Fonts->AddFontDefault();
    return false;
  }

  io.FontDefault = font;
  std::cerr << "Loaded main font: " << fontPath << " @ " << sizePx
            << "px (density " << dpiScale << ")\n";
  return true;
}

// ---------------------------------------------------------------------------
// Content fonts (editor + preview)
// ---------------------------------------------------------------------------
namespace {
constexpr int kMaxContentFonts = 16;
ImFont *g_contentFonts[kMaxContentFonts] = {nullptr};
float g_contentSizes[kMaxContentFonts] = {0.0f};
int g_numContentFonts = 0;
int g_defaultContentFontIdx = 2;
} // namespace

void loadContentFonts(float dpiScale) {
  // Discrete sizes. Keep the middle roughly aligned with the UI font (16px).
  static const float kSizes[] = {12.0f, 14.0f, 16.0f, 18.0f,
                                 20.0f, 22.0f, 24.0f, 28.0f};
  static const int kNumSizes = (int)(sizeof(kSizes) / sizeof(kSizes[0]));
  static const int kDefaultIdx = 2; // 16px

  std::string fontPath = std::string(IMGUI_FONTS_DIR) + "/Cousine-Regular.ttf";
  if (!std::filesystem::exists(fontPath)) {
    std::cerr << "Error: content font not found at " << fontPath << "\n";
    return;
  }

  ImGuiIO &io = ImGui::GetIO();
  g_numContentFonts = 0;
  g_defaultContentFontIdx = kDefaultIdx;

  for (int i = 0; i < kNumSizes && g_numContentFonts < kMaxContentFonts; ++i) {
    ImFontConfig cfg;
    cfg.OversampleH = 2;
    cfg.OversampleV = 2;
    cfg.PixelSnapH = false;
    cfg.RasterizerDensity = dpiScale;
    cfg.FontDataOwnedByAtlas = true;

    ImFont *font =
        io.Fonts->AddFontFromFileTTF(fontPath.c_str(), kSizes[i], &cfg);
    if (!font) {
      std::cerr << "Failed to load content font @ " << kSizes[i] << "px\n";
      continue;
    }
    g_contentFonts[g_numContentFonts] = font;
    g_contentSizes[g_numContentFonts] = kSizes[i];
    ++g_numContentFonts;
  }
  std::cerr << "Loaded " << g_numContentFonts << " content font sizes\n";
}

ImFont *getContentFont(int idx) {
  if (idx < 0 || idx >= g_numContentFonts)
    idx = g_defaultContentFontIdx;
  if (idx < 0 || idx >= g_numContentFonts)
    return nullptr;
  return g_contentFonts[idx];
}

int getContentFontCount() { return g_numContentFonts; }
int getDefaultContentFontIdx() { return g_defaultContentFontIdx; }

float getContentFontSize(int idx) {
  if (idx < 0 || idx >= g_numContentFonts)
    return 16.0f;
  return g_contentSizes[idx];
}
