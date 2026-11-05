#pragma once

#include "datamodel.h"

void viewToggleButton(const char *icon, const char *tip, ViewMode current,
                      ViewMode target, ViewMode &out);
