#pragma once

#include <imgui.h>

void glfwErrorCallback(int err, const char *desc);
bool loadIconFont(float sizePx, float dpiScale);
bool loadMainFont(float sizePx, float dpiScale);

void loadContentFonts(float dpiScale);
ImFont *getContentFont(int idx);
int getContentFontCount();
int getDefaultContentFontIdx();
float getContentFontSize(int idx);
