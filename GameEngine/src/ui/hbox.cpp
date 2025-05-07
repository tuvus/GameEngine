#include "ui/eui.h"
#include <algorithm>
#include <iostream>

void EUI_HBox::Layout(EUI_Context& ctx) {
    Alignment main_axis_alignment = Get_Horizontal_Alignment(ctx);

    float cursor = pos.x + padding.x;
    float total_content_width = 0;

    // gap between elements in container
    float total_gap = 0;
    if (gap)
        total_gap = gap * (children.size() - 1);

    // gap for align-stretch
    float interval = 0;

    // for auto-sizing nested containers
    float spacing = 0;
    if (children.size())
        spacing = (dim.x - padding.x * 2) / children.size();

    // layout children and calculate total content height
    for (EUI_Element* child : children) {
        /* TODO:if (ABSOLUTE) continue; */
        if (child->Is_Container()) {
            child->pos = {cursor, pos.y + padding.y};
            child->dim = {spacing, dim.y - padding.y * 2};
        }
        child->Layout(ctx);
        total_content_width += child->preferred_size.x;
        cursor += spacing;
    }

    // pick starting cursor location
    cursor = pos.x + padding.x;
    switch (main_axis_alignment) {
        case Alignment::Center:
            cursor = pos.x + (dim.x - total_content_width - total_gap) / 2.0f;
            break;
        case Alignment::End:
            cursor = pos.x + dim.x - total_content_width - total_gap - padding.x;
            break;
        case Alignment::Stretch:
            interval = (dim.x - total_content_width - padding.x * 2) / (children.size() - 1);
            break;
        case Alignment::Start:
            break;
    }

    for (int i = 0; i < children.size(); i++) {
        EUI_Element* child = children[i];
        if (child->Is_Container())
            continue;

        // clamp to min/max
        float width = std::clamp(child->preferred_size.x, child->min_size.x, child->max_size.x);
        float height = std::clamp(child->preferred_size.y, child->min_size.y, child->max_size.y);

        float x = cursor;
        float y = pos.y + padding.y;

        // cross axis alignment
        switch (child->Get_Vertical_Alignment(ctx)) {
            case Alignment::Center:
                y = pos.y + (dim.y - height) / 2.0f;
                break;
            case Alignment::End:
                y = pos.y + (dim.y - height - padding.y);
                break;
            case Alignment::Stretch:
            case Alignment::Start:
            default:
                break;
        }

        // Final bounds
        child->pos = {x, y};
        child->dim = {width, height};

        // Advance along main axis
        if (gap && i != children.size() - 1)
            cursor += gap;
        if (interval)
            cursor += interval;
        cursor += width;
    }
}
