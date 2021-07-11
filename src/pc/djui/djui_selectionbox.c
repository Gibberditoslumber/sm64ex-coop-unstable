#include <stdio.h>
#include <string.h>
#include "djui.h"

ALIGNED8 static u8 texture_selectionbox_icon[] = {
#include "textures/segment2/custom_selectionbox_icon.rgba16.inc.c"
};

static void djui_selectionbox_set_default_style(struct DjuiBase* base) {
    struct DjuiSelectionbox* selectionbox = (struct DjuiSelectionbox*)base;
    djui_base_set_border_color(&selectionbox->rect->base, 150, 150, 150, 255);
    djui_base_set_color(&selectionbox->rect->base, 200, 200, 200, 255);
    djui_base_set_color(&selectionbox->rectText->base, 11, 11, 11, 255);
    djui_base_set_location(&selectionbox->rectText->base, 0.0f, 3.0f);
    djui_base_set_color(&selectionbox->rectImage->base, 0, 0, 0, 255);
    djui_base_set_color(&selectionbox->text->base, 200, 200, 200, 255);
}

static void djui_selectionbox_on_hover(struct DjuiBase* base) {
    struct DjuiSelectionbox* selectionbox = (struct DjuiSelectionbox*)base;
    f32 x = selectionbox->rect->base.elem.x;
    if (gCursorX >= x) {
        djui_base_set_border_color(&selectionbox->rect->base, 0, 120, 215, 255);
        djui_base_set_color(&selectionbox->rect->base, 229, 241, 251, 255);
        djui_base_set_location(&selectionbox->rectText->base, -1.0f, 2.0f);
        djui_base_set_color(&selectionbox->text->base, 229, 241, 251, 255);
    } else {
        djui_selectionbox_set_default_style(base);
    }
}

static void djui_selectionbox_on_hover_end(struct DjuiBase* base) {
    djui_selectionbox_set_default_style(base);
}

static void djui_selectionbox_on_cursor_down_begin(struct DjuiBase* base, bool inputCursor) {
    struct DjuiSelectionbox* selectionbox = (struct DjuiSelectionbox*)base;
    f32 x = selectionbox->rect->base.elem.x;
    if (gCursorX >= x) {
        djui_base_set_border_color(&selectionbox->rect->base, 0, 84, 153, 255);
        djui_base_set_color(&selectionbox->rect->base, 204, 228, 247, 255);
        djui_base_set_location(&selectionbox->rectText->base, 0.5f, 3.5f);
        djui_base_set_color(&selectionbox->text->base, 229, 241, 251, 255);

        *selectionbox->value = (*selectionbox->value + 1) % selectionbox->choiceCount;
        djui_text_set_text(selectionbox->rectText, selectionbox->choices[*selectionbox->value]);
        if (base != NULL && base->interactable != NULL && base->interactable->on_value_change != NULL) {
            base->interactable->on_value_change(base);
        }
    }
    else {
        djui_selectionbox_set_default_style(base);
    }
}

static void djui_selectionbox_on_cursor_down_end(struct DjuiBase* base) {
    struct DjuiSelectionbox* selectionbox = (struct DjuiSelectionbox*)base;
    djui_selectionbox_set_default_style(base);
}

static void djui_selectionbox_destroy(struct DjuiBase* base) {
    struct DjuiSelectionbox* selectionbox = (struct DjuiSelectionbox*)base;
    for (int i = 0; i < selectionbox->choiceCount; i++) {
        free(selectionbox->choices[i]);
    }
    free(selectionbox->choices);
    free(selectionbox);
}

struct DjuiSelectionbox* djui_selectionbox_create(struct DjuiBase* parent, const char* message, char* choices[], u8 choiceCount, unsigned int* value) {
    struct DjuiSelectionbox* selectionbox = malloc(sizeof(struct DjuiSelectionbox));
    struct DjuiBase* base     = &selectionbox->base;

    selectionbox->value = value;
    selectionbox->choices = malloc(sizeof(char*) * choiceCount);
    for (int i = 0; i < choiceCount; i++) {
        u32 length = strlen(choices[i]);
        selectionbox->choices[i] = malloc(sizeof(char) * (length + 1));
        memset(selectionbox->choices[i], 0, sizeof(char) * (length + 1));
        sprintf(selectionbox->choices[i], "%s", choices[i]);
    }
    selectionbox->choiceCount = choiceCount;

    djui_base_init(parent, base, NULL, djui_selectionbox_destroy);
    djui_interactable_create(base);
    djui_interactable_hook_hover(base, djui_selectionbox_on_hover, djui_selectionbox_on_hover_end);
    djui_interactable_hook_cursor_down(base, djui_selectionbox_on_cursor_down_begin, NULL, djui_selectionbox_on_cursor_down_end);

    struct DjuiText* text = djui_text_create(&selectionbox->base, message);
    djui_base_set_alignment(&text->base, DJUI_HALIGN_LEFT, DJUI_VALIGN_CENTER);
    djui_base_set_size_type(&text->base, DJUI_SVT_RELATIVE, DJUI_SVT_RELATIVE);
    djui_base_set_size(&text->base, 0.5f, 1.0f);
    djui_text_set_alignment(text, DJUI_HALIGN_LEFT, DJUI_VALIGN_BOTTOM);
    djui_text_set_drop_shadow(text, 120, 120, 120, 64);
    selectionbox->text = text;

    struct DjuiRect* rect = djui_rect_create(&selectionbox->base);
    djui_base_set_alignment(&rect->base, DJUI_HALIGN_RIGHT, DJUI_VALIGN_CENTER);
    djui_base_set_size_type(&rect->base, DJUI_SVT_RELATIVE, DJUI_SVT_RELATIVE);
    djui_base_set_size(&rect->base, 0.5f, 1.0f);
    djui_base_set_color(&rect->base, 0, 0, 0, 0);
    djui_base_set_border_width(&rect->base, 2);
    djui_base_set_padding(&rect->base, 2, 2, 0, 4);
    selectionbox->rect = rect;

    struct DjuiText* rectText = djui_text_create(&rect->base, choices[*value]);
    djui_base_set_alignment(&rectText->base, DJUI_HALIGN_LEFT, DJUI_VALIGN_CENTER);
    djui_base_set_size_type(&rectText->base, DJUI_SVT_RELATIVE, DJUI_SVT_RELATIVE);
    djui_base_set_size(&rectText->base, 1.0f, 1.0f);
    djui_text_set_alignment(rectText, DJUI_HALIGN_LEFT, DJUI_VALIGN_BOTTOM);
    djui_text_set_drop_shadow(rectText, 120, 120, 120, 64);
    selectionbox->rectText = rectText;

    struct DjuiImage* rectImage = djui_image_create(&rect->base, texture_selectionbox_icon, 16, 16, 16);
    djui_base_set_location(&rectImage->base, 0, 0);
    djui_base_set_size(&rectImage->base, 16, 16);
    djui_base_set_alignment(&rectImage->base, DJUI_HALIGN_RIGHT, DJUI_VALIGN_CENTER);
    selectionbox->rectImage = rectImage;

    djui_selectionbox_set_default_style(base);

    return selectionbox;
}