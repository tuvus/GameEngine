#include "ui/eui.h"
#include <raylib.h>

/* EUI_Context Implementations */
void EUI_Context::Update_Input()
{
    input.mouse_position = GetMousePosition();
    input.left_mouse_down = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
    input.left_mouse_pressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    input.right_mouse_down = IsMouseButtonDown(MOUSE_BUTTON_RIGHT);
    input.right_mouse_pressed = IsMouseButtonPressed(MOUSE_BUTTON_RIGHT);
    input.key_pressed = GetKeyPressed();
}
void EUI_Context::Begin_Frame()
{
    hovered = nullptr;
    active = nullptr;
}
void EUI_Context::End_Frame()
{
    input.key_pressed = -1;
}
void EUI_Context::Perform_Layout()
{
    if (root)
        root->Layout(*this);
}
void EUI_Context::Handle_Input()
{
    if (root)
        root->Handle_Input(*this);
}
void EUI_Context::Render()
{
    if (root)
        root->Render(*this);
}

/* EUI_ELement Implementations */
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

EUI_Style EUI_Element::Get_Effective_Style(const EUI_Context& ctx) const
{
    return {
        .background_color = Get_Background_Color(ctx),
        .text_color = Get_Text_Color(ctx),
        .border_color = Get_Border_Color(ctx),
        .border_radius = Get_Border_Radius(ctx),
        .font = Get_Font(ctx),
    };
}
