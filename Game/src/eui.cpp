#include "eui.h"

Color EUI_Element::Get_Background_Color(const EUI_Context& ctx) const
{
    if (style.background_color)
        return *style.background_color;
    if (parent)
        return parent->Get_Background_Color(ctx);
    return *ctx.default_style.background_color;
}
Color EUI_Element::Get_Text_Color(const EUI_Context& ctx) const
{
    if (style.text_color)
        return *style.text_color;
    if (parent)
        return parent->Get_Text_Color(ctx);
    return *ctx.default_style.text_color;
};
Color EUI_Element::Get_Border_Color(const EUI_Context& ctx) const
{
    if (style.border_color)
        return *style.border_color;
    if (parent)
        return parent->Get_Border_Color(ctx);
    return *ctx.default_style.border_color;
};
float EUI_Element::Get_Border_Radius(const EUI_Context& ctx) const
{
    if (style.border_radius)
        return *style.border_radius;
    if (parent)
        return parent->Get_Border_Radius(ctx);
    return *ctx.default_style.border_radius;
};
float EUI_Element::Get_Border_Thickness(const EUI_Context& ctx) const
{
    if (style.border_thickness)
        return *style.border_thickness;
    if (parent)
        return parent->Get_Border_Thickness(ctx);
    return *ctx.default_style.border_thickness;
};
Font EUI_Element::Get_Font(const EUI_Context& ctx) const
{
    if (style.font)
        return *style.font;
    if (parent)
        return parent->Get_Font(ctx);
    return *ctx.default_style.font;
};

EUI_Style EUI_Element::Get_Effective_Style(const EUI_Context& ctx)
{
    return {
        .background_color = Get_Background_Color(ctx),
        .text_color = Get_Text_Color(ctx),
        .border_color = Get_Border_Color(ctx),
        .border_radius = Get_Border_Radius(ctx),
        .font = Get_Font(ctx),
    };
}
