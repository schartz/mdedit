#pragma once

#include "datamodel.h"

void renderSidebar(AppState &app);

// Drops the cached sidebar directory snapshot; the tree is re-scanned on the
// next frame. Call after operations that can add/remove files on disk.
void invalidateSidebarCache();
