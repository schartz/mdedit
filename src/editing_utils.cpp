#include "editing_utils.h"

void findNext(AppState &app) {
  auto &fr = app.find_replace;
  if (app.active < 0) {
    fr.status = "No document";
    return;
  }
  if (fr.find_term.empty()) {
    fr.status = "Enter a search term";
    return;
  }

  auto &text = app.docs[app.active].content;
  size_t start = (fr.last_match >= 0) ? (size_t)fr.last_match + 1 : 0;
  size_t pos = text.find(fr.find_term, start);
  if (pos == std::string::npos && start > 0)
    pos = text.find(fr.find_term);
  if (pos == std::string::npos) {
    fr.last_match = -1;
    fr.status = "No match";
  } else {
    fr.last_match = (int)pos;
    fr.status = "Match at " + std::to_string(pos);
  }
}

void replaceCurrent(AppState &app) {
  auto &fr = app.find_replace;
  if (app.active < 0 || fr.last_match < 0)
    return;
  auto &doc = app.docs[app.active];
  doc.content.replace(fr.last_match, fr.find_term.size(), fr.replace_term);
  doc.dirty = true;
  fr.last_match = -1;
  fr.status = "Replaced";
}

void replaceAll(AppState &app) {
  auto &fr = app.find_replace;
  if (app.active < 0 || fr.find_term.empty())
    return;
  auto &doc = app.docs[app.active];
  size_t count = 0, pos = 0;
  while ((pos = doc.content.find(fr.find_term, pos)) != std::string::npos) {
    doc.content.replace(pos, fr.find_term.size(), fr.replace_term);
    pos += fr.replace_term.size();
    ++count;
  }
  if (count)
    doc.dirty = true;
  fr.status = "Replaced " + std::to_string(count) + " occurrence(s)";
}
