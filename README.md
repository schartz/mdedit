# MdNotes

Minimal markdown notes editor built with Dear ImGui.

## Features

- Split, editor-only, and preview-only view modes
- Live markdown preview via imgui_rich_md
- File tree sidebar rooted at a configurable directory
- Multi-document tabs with dirty-state tracking
- Find / replace (single and replace-all)
- Autosave to disk on a configurable interval
- Session persistence: open docs, recent files, view mode, sidebar root
- Native file, folder, and message dialogs (tinyfiledialogs)
- Feather icon font via icomoon

## Requirements

- CMake 3.16+
- C++17 compiler
- OpenGL 3.3+
- glfw3 (system package)
- Linux: any tinyfiledialogs backend available (zenity, kdialog, yad, etc.)
  or a console fallback

## Build

    cmake -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
    cmake --build build -j"$(nproc)"
    ./build/md_notes

Runtime assets must exist under `assets/` (icomoon.ttf, appicon.png);
the path is baked in at configure time via `MDNOTES_ASSETS_DIR`.

## Configuration

Session state is written to `$XDG_CONFIG_HOME/mdnotes/session.json`
(falling back to `~/.config/mdnotes/session.json`). It records recent
files, open documents, sidebar root, and view mode. Delete the file to
reset state; do not edit by hand.


## Third-party

Fetched at configure time: Dear ImGui, imgui_rich_md, nlohmann/json, stb.
Vendored: tinyfiledialogs v3.22.1 (SourceForge). Sits in `thirdparty/tinyfd` directory.

## Notes

- Not at all mature.
- Not recommended to be used as of now.