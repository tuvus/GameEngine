#include "ui/eui.h"

void EUI_Button::Layout(EUI_Context& ctx)
{
    const auto effective_style = Get_Effective_Style(ctx);

    Vector2 text_size =
        MeasureTextEx(effective_style.font.value(), text.c_str(), effective_style.font_size.value(),
                      effective_style.font_spacing.value());

    float width = padding.x * 2 + text_size.x;
    float height = padding.y * 2 + text_size.y;

    bounds.width = preferred_size.x > 0 ? preferred_size.x : width;
    bounds.height = preferred_size.y > 0 ? preferred_size.y : height;
}

void EUI_Button::Handle_Input(EUI_Context& ctx)
{
    if (!visible)
        return;

    hovered = CheckCollisionPointRec(ctx.input.mouse_position, bounds);
    if (hovered)
        ctx.hovered = this;

    if (hovered && ctx.input.left_mouse_pressed)
    {
        active = true;
        ctx.active = this;
    }

    if (active && !ctx.input.left_mouse_down && hovered)
    {
        if (on_click)
            on_click(); // Execute callback
        active = false;
    }
}

void EUI_Button::Render(EUI_Context& ctx)
{
    if (!visible)
        return;

    const auto style = Get_Effective_Style(ctx);

    // Background
    DrawRectangleRec(bounds, style.background_color.value());

    // Border
    if (style.border_thickness.value() > 0)
    {
        DrawRectangleLinesEx(bounds, style.border_thickness.value(), style.border_color.value());
    }

    // Text
    Vector2 text_size =
        MeasureTextEx(style.font.value(), text.c_str(), 16, 1); // consistent with Layout
    Vector2 text_pos = {bounds.x + (bounds.width - text_size.x) / 2,
                        bounds.y + (bounds.height - text_size.y) / 2};
    DrawTextEx(style.font.value(), text.c_str(), text_pos, 16, 1, style.text_color.value());
}
