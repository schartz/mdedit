#include "window_icon.h"
#include <filesystem>
#include <iostream>
#include <stb_image.h>

// Loads a PNG and applies it as the GLFW window icon.
// GLFW expects raw RGBA; stb_image does the PNG decoding for us.
void applyWindowIcon(GLFWwindow *window, const std::string &pngPath) {
#if GLFW_VERSION_MAJOR > 3 ||                                                  \
    (GLFW_VERSION_MAJOR == 3 && GLFW_VERSION_MINOR >= 4)
  if (glfwGetPlatform() == GLFW_PLATFORM_WAYLAND) {
    std::cerr << "Info: GLFW cannot set the window icon on Wayland; skipping.\n"
                 "      (See README/notes for a .desktop-file workaround.)\n";
    return;
  }
#endif

  if (!std::filesystem::exists(pngPath)) {
    std::cerr << "Warning: window icon not found at " << pngPath << "\n";
    return;
  }

  int w = 0, h = 0, channels = 0;
  unsigned char *pixels = stbi_load(pngPath.c_str(), &w, &h, &channels, 4);
  if (!pixels) {
    std::cerr << "Warning: could not decode '" << pngPath
              << "': " << stbi_failure_reason() << "\n";
    return;
  }

  GLFWimage img;
  img.width = w;
  img.height = h;
  img.pixels = pixels;
  glfwSetWindowIcon(window, 1, &img); // GLFW copies the data
  stbi_image_free(pixels);

  std::cerr << "Set window icon from " << pngPath << " (" << w << "x" << h
            << ")\n";
}
