#include "ui/eui.h"

#define RETURN_STYLE_PROP(prop, getter)                                                            \
    if (style.prop.has_value())                                                                    \
        return style.prop.value();                                                                 \
    if (parent)                                                                                    \
        return parent->getter(ctx);                                                                \
    return ctx.default_style.prop.value();

/* EUI_Element Implementations */
Color EUI_Element::Get_Text_Color(const EUI_Context& ctx) const {
    RETURN_STYLE_PROP(text_color, Get_Text_Color);
}

Font EUI_Element::Get_Font(const EUI_Context& ctx) const {
    RETURN_STYLE_PROP(font, Get_Font);
}
float EUI_Element::Get_Font_Size(const EUI_Context& ctx) const {
    RETURN_STYLE_PROP(font_size, Get_Font_Size);
}
float EUI_Element::Get_Font_Spacing(const EUI_Context& ctx) const {
    RETURN_STYLE_PROP(font_spacing, Get_Font_Spacing);
}

Alignment EUI_Element::Get_Horizontal_Alignment(const EUI_Context& ctx) const {
    RETURN_STYLE_PROP(horizontal_alignment, Get_Horizontal_Alignment);
}

Alignment EUI_Element::Get_Vertical_Alignment(const EUI_Context& ctx) const {
    RETURN_STYLE_PROP(vertical_alignment, Get_Vertical_Alignment);
}

Alignment EUI_Element::Get_Text_Horizontal_Alignment(const EUI_Context& ctx) const {
    RETURN_STYLE_PROP(text_horizontal_alignment, Get_Text_Horizontal_Alignment);
}

Alignment EUI_Element::Get_Text_Vertical_Alignment(const EUI_Context& ctx) const {
    RETURN_STYLE_PROP(text_vertical_alignment, Get_Text_Vertical_Alignment);
}

EUI_Style EUI_Element::Get_Effective_Style(const EUI_Context& ctx) const {
    EUI_Style effective;

    effective.background_color = style.background_color;
    effective.text_color = Get_Text_Color(ctx);

    effective.border_color = style.border_color;
    effective.border_radius = style.border_radius;

    effective.font = Get_Font(ctx);
    effective.font_size = Get_Font_Size(ctx);
    effective.font_spacing = Get_Font_Spacing(ctx);

    effective.horizontal_alignment = Get_Horizontal_Alignment(ctx);
    effective.vertical_alignment = Get_Vertical_Alignment(ctx);
    effective.text_horizontal_alignment = Get_Text_Horizontal_Alignment(ctx);
    effective.text_vertical_alignment = Get_Text_Vertical_Alignment(ctx);

    return effective;
}
