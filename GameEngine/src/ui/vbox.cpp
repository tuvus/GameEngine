#include "ui/eui.h"
#include <algorithm>
#include <iostream>

void EUI_VBox::Layout(EUI_Context& ctx) {
    Alignment main_axis_alignment = style.vertical_alignment;

    float cursor = pos.y + style.padding.top;
    float total_content_height = 0;
    float total_leaf_height = 0;

    // gap between elements in container
    float total_gap = 0;

    // gap for align-stretch
    float interval = 0;

    // for auto-sizing nested containers
    float default_spacing = 0;
    int num_containers = 0;

    int num_layout_children = 0;

    // calculate total non-container content height
    for (EUI_Element* child : children) {
        if (child->style.position == Position::Absolute) {
            continue;
        }
        if (child->Is_Container()) {
            num_containers++;
            continue;
        }
        child->Layout(ctx);
        total_leaf_height += child->preferred_size.y;
        num_layout_children++;
    }

    if (gap && num_layout_children)
        total_gap = gap * (num_layout_children - 1);

    if (children.size())
        default_spacing =
            (dim.y - style.padding.top - style.padding.bottom - total_leaf_height) / num_containers;

    // place containers
    for (EUI_Element* child : children) {
        if (child->Get_Effective_Style(ctx).position == Position::Absolute) {
            continue;
        }
        if (child->Is_Container()) {
            child->pos = {pos.x + style.padding.left, cursor};
            child->dim = {dim.x - style.padding.left - style.padding.right, default_spacing};
            child->preferred_size = child->dim;
            child->Layout(ctx);
        }
        total_content_height += child->preferred_size.y;
        cursor += child->preferred_size.y;
    }

    // pick starting cursor location
    cursor = pos.y + style.padding.top;
    switch (main_axis_alignment) {
        case Alignment::Center:
            cursor = pos.y + (dim.y - total_content_height - total_gap) / 2.0f;
            break;
        case Alignment::End:
            cursor = pos.y + dim.y - total_content_height - total_gap - style.padding.bottom;
            break;
        case Alignment::Stretch:
            interval = (dim.y - total_content_height - style.padding.top - style.padding.bottom) /
                       (children.size() - 1);
            break;
        case Alignment::Start:
            break;
    }

    for (int i = 0; i < children.size(); i++) {
        EUI_Element* child = children[i];

        if (!child->Is_Container() &&
            child->Get_Effective_Style(ctx).position != Position::Absolute) {
            // clamp to min/max
            float width = std::clamp(child->preferred_size.x, child->min_size.x, child->max_size.x);
            float height =
                std::clamp(child->preferred_size.y, child->min_size.y, child->max_size.y);

            float x = pos.x + style.padding.left;
            float y = cursor;

            // cross axis alignment
            switch (style.horizontal_alignment) {
                case Alignment::Center:
                    x = pos.x + (dim.x - width + style.padding.left - style.padding.right) / 2.0f;
                    break;
                case Alignment::End:
                    x = pos.x + (dim.x - width - style.padding.right);
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
