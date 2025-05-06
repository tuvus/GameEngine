#include "ui/eui.h"
#include <iostream>
#include <raylib.h>

void EUI_Button::Layout(EUI_Context& ctx)
{
    const auto effective_style = Get_Effective_Style(ctx);

    // MEASURETEXTEX BROKEN ???
    /*Vector2 text_size =*/
    /*    MeasureTextEx(effective_style.font.value(), text.c_str(),
     * effective_style.font_size.value(),*/
    /*                  effective_style.font_spacing.value());*/
    int text_size = MeasureText(text.c_str(), effective_style.font_size.value());

    float width = padding.x * 2 + text_size + effective_style.font_spacing.value() * text.length();
    float height = padding.y * 2 + effective_style.font_size.value();

    preferred_size = {width, height};

    // set min/max FINISH LATER
    if (min_size.x <= 0)
        min_size.x = 0;
    if (min_size.y <= 0)
        min_size.y = 0;

    if (max_size.x <= 0)
        max_size.x = 9999;
    if (max_size.y <= 0)
        max_size.y = 9999;
}

void EUI_Button::Handle_Input(EUI_Context& ctx)
{
    if (!visible)
        return;

    hovered = CheckCollisionPointRec(ctx.input.mouse_position, bounds);
    if (hovered)
    {
        ctx.hovered = this;
    }

    if (hovered && ctx.input.left_mouse_pressed)
    {
        active = true;
        ctx.active = this;
    }

    if (active && !ctx.input.left_mouse_down && hovered)
    {
        if (on_click)
            on_click();
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
    /*Vector2 text_size = MeasureTextEx(style.font.value(), text.c_str(), style.font_size.value(),*/
    /*                                  style.font_spacing.value());*/
    int text_size = MeasureText(text.c_str(), style.font_size.value());
    Vector2 text_pos = {
        bounds.x + (bounds.width - text_size /*- style.font_spacing.value() * text.length()*/) / 2,
        bounds.y + (bounds.height - style.font_size.value()) / 2};
    /*DrawTextEx(style.font.value(), text.c_str(), text_pos, style.font_size.value(),*/
    /*           style.font_spacing.value(), style.text_color.value());*/
    DrawText(text.c_str(), text_pos.x, text_pos.y, style.font_size.value(),
             style.text_color.value());
}
