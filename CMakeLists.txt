cmake_minimum_required(VERSION 3.16)
project(MdNotes LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

include(FetchContent)

# --- Dear ImGui ---
FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG v1.93.0)
FetchContent_MakeAvailable(imgui)

add_library(
  imgui STATIC
  ${imgui_SOURCE_DIR}/imgui.cpp
  ${imgui_SOURCE_DIR}/imgui_draw.cpp
  ${imgui_SOURCE_DIR}/imgui_tables.cpp
  ${imgui_SOURCE_DIR}/imgui_widgets.cpp
  ${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp)

target_include_directories(
  imgui PUBLIC ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends
               ${imgui_SOURCE_DIR}/misc/cpp ${imgui_SOURCE_DIR}/misc/fonts)

target_link_libraries(imgui PUBLIC glfw OpenGL::GL)

# --- imgui_rich_md ---
set(IMGUI_RICHMD_WITH_DOWNLOAD_IMAGES
    ON
    CACHE BOOL "" FORCE)
FetchContent_Declare(
  imgui_rich_md
  GIT_REPOSITORY https://github.com/pthom/imgui_rich_md.git
  GIT_TAG v1.0.0)
FetchContent_MakeAvailable(imgui_rich_md)

# --- tinyfiledialogs (single C file, no runtime deps) ---
# FetchContent_Declare( tinyfiledialogs GIT_REPOSITORY
# https://github.com/native-toolkit/libtinyfiledialogs.git GIT_TAG master)
# FetchContent_MakeAvailable(tinyfiledialogs)
#
# add_library(tinyfiledialogs STATIC
# ${tinyfiledialogs_SOURCE_DIR}/tinyfiledialogs.c)
# target_include_directories(tinyfiledialogs PUBLIC
# ${tinyfiledialogs_SOURCE_DIR}) if(WIN32)
# target_compile_definitions(tinyfiledialogs PRIVATE _CRT_SECURE_NO_WARNINGS)
# target_link_libraries(tinyfiledialogs Comdlg32 Ole32) endif()

set(TINYFD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/tinyfd)

add_library(tinyfiledialogs STATIC ${TINYFD_DIR}/tinyfiledialogs.c)
target_include_directories(tinyfiledialogs PUBLIC ${TINYFD_DIR})
if(WIN32)
  target_compile_definitions(tinyfiledialogs PRIVATE _CRT_SECURE_NO_WARNINGS)
  target_link_libraries(tinyfiledialogs Comdlg32 Ole32 user32 shell32 gdi32)
endif()

# --- nlohmann/json (header-only) ---
FetchContent_Declare(
  json
  URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

# --- stb_image (single-header PNG decoder) ---
FetchContent_Declare(
  stb
  GIT_REPOSITORY https://github.com/nothings/stb.git
  GIT_TAG master)
FetchContent_MakeAvailable(stb)

# --- Application ---
add_library(
  mdnotes_core STATIC src/fs_utils.cpp src/session.cpp src/documents.cpp
                      src/editing_utils.cpp src/commands.cpp)
target_link_libraries(mdnotes_core PUBLIC tinyfiledialogs
                                          nlohmann_json::nlohmann_json)

add_executable(
  md_notes
  src/main.cpp
  src/window_icon.cpp
  src/fonts.cpp
  src/ui/theme.cpp
  src/ui/debug_utils.cpp
  src/ui/document_view.cpp
  src/ui/sidebar.cpp
  src/ui/toolbar.cpp)

target_link_libraries(md_notes PRIVATE mdnotes_core imgui imgui_rich_md glfw
                                       OpenGL::GL)

target_include_directories(md_notes PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
                                            ${stb_SOURCE_DIR})

target_compile_definitions(
  md_notes PRIVATE IMGUI_FONTS_DIR="${imgui_SOURCE_DIR}/misc/fonts"
                   MDNOTES_ASSETS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets")
