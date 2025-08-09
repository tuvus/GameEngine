#include "ui/eui.h"

EUI_Container::~EUI_Container() {
    for (EUI_Element* child : children) {
        delete child;
    }
}

void EUI_Container::Add_Child(EUI_Element* child) {
    if (!child)
        return;

    child->parent = this;
    children.push_back(child);
    if (context) {
        child->Set_Context(*context);
    }
}

void EUI_Container::Set_Context(EUI_Context& ctx) {
    context = &ctx;
    for (auto* child : children) {
        child->Set_Context(ctx);
    }
}

void EUI_Container::Handle_Input(EUI_Context& ctx) {
    if (!is_visible || is_deleted)
        return;

    for (EUI_Element* child : children) {
        if (child->is_visible) {
            child->Handle_Input(ctx);
            if (is_deleted)
                return;
        }
    }
}

void EUI_Container::Render(EUI_Context& ctx) {
    if (!is_visible)
        return;

    const auto style = Get_Effective_Style(ctx);

    if (style.background_color.has_value())
        DrawRectangleRec({pos.x, pos.y, dim.x, dim.y}, style.background_color.value());

    if (style.border_radius > 0)
        DrawRectangleLinesEx({pos.x, pos.y, dim.x, dim.y}, style.border_radius, style.border_color);

    // Render children
    for (EUI_Element* child : children) {
        if (child->is_visible)
            child->Render(ctx);
    }
}
