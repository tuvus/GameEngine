#include "ui/eui.h"
#include <algorithm>

void EUI_VBox::Layout(EUI_Context& ctx) {
    Alignment main_axis_alignment = Get_Vertical_Alignment(ctx);

    float cursor = pos.y + padding.top;
    float total_content_height = 0;

    // gap between elements in container
    float total_gap = gap * (children.size() - 1);

    // gap for align-stretch
    float interval = 0;

    // for auto-sizing nested containers
    float default_spacing = 0;
    if (Is_Container() && children.size())
        default_spacing = (dim.y - padding.top - padding.bottom) / children.size();

    // layout children and calculate total content height
    for (EUI_Element* child : children) {
        if (child->Is_Container()) {
            child->pos = {pos.x + padding.left, cursor};
            child->dim = {dim.x - padding.left - padding.right, default_spacing};
        }
        child->Layout(ctx);
        total_content_height += child->preferred_size.y;
        cursor += default_spacing;
    }

    // pick starting cursor location
    cursor = pos.y + padding.top;
    switch (main_axis_alignment) {
        case Alignment::Center:
            cursor = pos.y + (dim.y - total_content_height - total_gap) / 2.0f;
            break;
        case Alignment::End:
            cursor = pos.y + dim.y - total_content_height - total_gap - padding.bottom;
            break;
        case Alignment::Stretch:
            interval = (dim.y - total_content_height - padding.top - padding.bottom) /
                       (children.size() - 1);
            break;
        case Alignment::Start:
            break;
    }

    for (int i = 0; i < children.size(); i++) {
        EUI_Element* child = children[i];

        if (!child->Is_Container()) {
            // clamp to min/max
            float width = std::clamp(child->preferred_size.x, child->min_size.x, child->max_size.x);
            float height =
                std::clamp(child->preferred_size.y, child->min_size.y, child->max_size.y);

            float x = pos.x + padding.left;
            float y = cursor;

            // cross axis alignment
            switch (child->Get_Horizontal_Alignment(ctx)) {
                case Alignment::Center:
                    x = pos.x + (dim.x - width + padding.left - padding.right) / 2.0f;
                    break;
                case Alignment::End:
                    x = pos.x + (dim.x - width - padding.right);
                    break;
                case Alignment::Stretch:
                case Alignment::Start:
                default:
                    break;
            }

            // Final bounds
            child->pos = {x, y};
            child->dim = {width, height};
        }

        // Advance along main axis
        if (gap && i != children.size() - 1)
            cursor += gap;
        if (interval)
            cursor += interval;
        cursor += child->dim.y;
    }
}
