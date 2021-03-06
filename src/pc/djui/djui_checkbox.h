#pragma once
#include "djui.h"

#pragma pack(1)
struct DjuiCheckbox {
    struct DjuiBase base;
    struct DjuiRect* rect;
    struct DjuiRect* rectValue;
    struct DjuiText* text;
    bool* value;
};

struct DjuiCheckbox* djui_checkbox_create(struct DjuiBase* parent, const char* message, bool* value);
