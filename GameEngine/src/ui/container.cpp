#include "ui/eui.h"

void EUI_Container::Layout(EUI_Context& ctx)
{
    const auto effective_style = Get_Effective_Style(ctx);

    float width = padding.x * 2;
    float height = padding.y * 2;

    bounds.width = preferred_size.x > 0 ? preferred_size.x : width;
    bounds.height = preferred_size.y > 0 ? preferred_size.y : height;
}

void EUI_Container::Handle_Input(EUI_Context& ctx)
{
    if (!visible)
        return;
}

void EUI_Container::Render(EUI_Context& ctx)
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
}
