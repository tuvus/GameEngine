#include "ui/eui.h"

/* EUI_Element Implementations */
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

Alignment EUI_Element::Get_Horizontal_Alignment(const EUI_Context& ctx) const
{
    if (style.horizontal_alignment)
        return *style.horizontal_alignment;
    if (parent)
        return parent->Get_Horizontal_Alignment(ctx);
    return ctx.default_style.horizontal_alignment.value_or(Alignment::Start);
}

Alignment EUI_Element::Get_Vertical_Alignment(const EUI_Context& ctx) const
{
    if (style.vertical_alignment)
        return *style.vertical_alignment;
    if (parent)
        return parent->Get_Vertical_Alignment(ctx);
    return ctx.default_style.vertical_alignment.value_or(Alignment::Start);
}

EUI_Style EUI_Element::Get_Effective_Style(const EUI_Context& ctx) const
{
    EUI_Style effective;

    // Background color
    if (style.background_color)
        effective.background_color = style.background_color;
    else if (parent)
        effective.background_color = parent->Get_Background_Color(ctx);
    else
        effective.background_color = ctx.default_style.background_color;

    // Text color
    if (style.text_color)
        effective.text_color = style.text_color;
    else if (parent)
        effective.text_color = parent->Get_Text_Color(ctx);
    else
        effective.text_color = ctx.default_style.text_color;

    // Border color
    if (style.border_color)
        effective.border_color = style.border_color;
    else if (parent)
        effective.border_color = parent->Get_Border_Color(ctx);
    else
        effective.border_color = ctx.default_style.border_color;

    // Border radius
    if (style.border_radius)
        effective.border_radius = style.border_radius;
    else if (parent)
        effective.border_radius = parent->Get_Border_Radius(ctx);
    else
        effective.border_radius = ctx.default_style.border_radius;

    // Border thickness
    if (style.border_thickness)
        effective.border_thickness = style.border_thickness;
    else if (parent)
        effective.border_thickness = parent->Get_Border_Thickness(ctx);
    else
        effective.border_thickness = ctx.default_style.border_thickness;

    // Font
    if (style.font)
        effective.font = style.font;
    else if (parent)
        effective.font = parent->Get_Font(ctx);
    else
        effective.font = ctx.default_style.font;

    if (style.horizontal_alignment)
        effective.horizontal_alignment = style.horizontal_alignment;
    else if (parent)
        effective.horizontal_alignment = parent->Get_Horizontal_Alignment(ctx);
    else
        effective.horizontal_alignment = ctx.default_style.horizontal_alignment;

    if (style.vertical_alignment)
        effective.vertical_alignment = style.vertical_alignment;
    else if (parent)
        effective.vertical_alignment = parent->Get_Vertical_Alignment(ctx);
    else
        effective.vertical_alignment = ctx.default_style.vertical_alignment;

    return effective;
}
