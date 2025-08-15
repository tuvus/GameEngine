#include "ui/eui.h"
#include <raylib.h>

EUI_Button::EUI_Button(const std::string& text, std::function<void()> on_click)
    : EUI_Text(text), on_click(on_click) {
    // default button styles
    style.border_radius = 1;
    style.border_color = BLACK;

    style.text_horizontal_alignment = Alignment::Center;
    style.text_vertical_alignment = Alignment::Center;
}

void EUI_Button::Handle_Input() {
    if (!ctx || !is_visible)
        return;

    is_hovered = CheckCollisionPointRec(ctx->input.mouse_position, {pos.x, pos.y, dim.x, dim.y});
    if (is_hovered) {
        ctx->hovered = this;
    } else {
        is_active = false;
    }

    if (is_hovered && ctx->input.left_mouse_pressed) {
        is_active = true;
        ctx->active = this;
    }

    if (is_active && !ctx->input.left_mouse_down && is_hovered) {
        if (on_click)
            on_click();
        is_active = false;
    }
}
