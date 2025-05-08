#include "ui/eui.h"

EUI_Text::EUI_Text(const std::string& text) : text(text) {
    // default styles
    style.border_thickness = 0;
    style.border_radius = 0;

    style.text_horizontal_alignment = Alignment::Center;
    style.text_vertical_alignment = Alignment::Center;
}

void EUI_Text::Layout(EUI_Context& ctx) {
    EUI_Style style = Get_Effective_Style(ctx);

    float text_width = MeasureText(text.c_str(), style.font_size.value());
    float text_height = style.font_size.value();

    float width =
        text_width + style.font_spacing.value() * text.length() + padding.left + padding.right;
    float height = text_height + padding.top + padding.bottom;

    preferred_size = {width, height};

    min_size = {text_width, text_height};
    max_size = {9999, 9999};
}

void EUI_Text::Handle_Input(EUI_Context& ctx) {
    // no op for now
}

void EUI_Text::Render(EUI_Context& ctx) {
    if (!is_visible)
        return;

    const auto style = Get_Effective_Style(ctx);

    // Background
    if (style.background_color.has_value())
        DrawRectangleRec({pos.x, pos.y, dim.x, dim.y}, style.background_color.value());

    // Border
    if (style.border_thickness.has_value() && style.border_thickness.value() > 0)
        DrawRectangleLinesEx({pos.x, pos.y, dim.x, dim.y}, style.border_thickness.value(),
                             style.border_color.value());

    // Text
    float text_width = MeasureText(text.c_str(), style.font_size.value());
    float text_height = style.font_size.value();

    switch (style.text_vertical_alignment.value()) {
        case Alignment::Center:
            text_pos.y = pos.y + (dim.y - text_height) / 2.0f;
            break;
        case Alignment::End:
            text_pos.y = pos.y + dim.y - text_height - padding.bottom;
            break;
        case Alignment::Stretch:
        case Alignment::Start:
            text_pos.y = pos.y;
    }
    switch (style.text_horizontal_alignment.value()) {
        case Alignment::Center:
            text_pos.x = pos.x + (dim.x - text_width) / 2;
            break;
        case Alignment::End:
            text_pos.x = pos.x + dim.x - text_width - padding.right;
            break;
        case Alignment::Stretch:
        case Alignment::Start:
            text_pos.x = pos.x;
    }

    DrawText(text.c_str(), text_pos.x, text_pos.y, style.font_size.value(),
             style.text_color.value());
}
