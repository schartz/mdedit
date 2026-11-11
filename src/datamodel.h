#pragma once

#include <string>
#include <vector>

struct Document {
  int id = 0;
  std::string path;
  std::string title;
  std::string content;
  bool dirty = false;
};

enum class ViewMode { Split, EditOnly, PreviewOnly };

struct FindReplaceState {
  bool open = false;
  bool focus_find = false;
  std::string find_term;
  std::string replace_term;
  int last_match = -1;
  std::string status;
};

struct AppState {
  std::vector<Document> docs;
  int active = -1;
  int nextDocId = 1;
  int untitledCounter = 1;
  ViewMode viewMode = ViewMode::Split;

  std::vector<std::string> recent_files;

  std::string sidebar_root;
  bool sidebar_visible = true;

  int content_font_idx = 2;

  float autosave_interval = 30.0f;
  float autosave_timer = 0.0f;

  FindReplaceState find_replace;
};
