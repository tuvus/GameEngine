#include "ui/eui.h"
#include <algorithm>

void EUI_Container::Add_Child(EUI_Element* child)
{
    if (!child)
        return;

    child->parent = this;
    children.push_back(child);
}

void EUI_Container::Layout(EUI_Context& ctx)
{
    bool vertical = layout_model == Layout_Model::Vertical;
    float cursor = vertical ? bounds.y + padding.y : bounds.x + padding.x;
    // TODO: make cursor center based on children??

    for (EUI_Element* child : children)
    {
        child->Layout(ctx);

        // Clamp to min/max
        float width = std::clamp(child->preferred_size.x, child->min_size.x, child->max_size.x);
        float height = std::clamp(child->preferred_size.y, child->min_size.y, child->max_size.y);

        float x = bounds.x + padding.x;
        float y = bounds.y + padding.y;

        // --- Main axis alignment ---
        if (vertical)
        {
            switch (child->Get_Vertical_Alignment(ctx))
            {
                case Alignment::Center:
                    y = cursor + (height - child->preferred_size.y) / 2.0f;
                    break;
                case Alignment::End:
                    y = cursor + (height - child->preferred_size.y);
                    break;
                case Alignment::Stretch:
                    height = std::max(height, child->preferred_size.y);
                    break;
                case Alignment::Start:
                default:
                    y = cursor;
                    break;
            }
        }
        else // Horizontal layout
        {
            switch (child->Get_Horizontal_Alignment(ctx))
            {
                case Alignment::Center:
                    x = cursor + (width - child->preferred_size.x) / 2.0f;
                    break;
                case Alignment::End:
                    x = cursor + (width - child->preferred_size.x);
                    break;
                case Alignment::Stretch:
                    width = std::max(width, child->preferred_size.x);
                    break;
                case Alignment::Start:
                default:
                    x = cursor;
                    break;
            }
        }

        // --- Cross axis alignment ---
        if (vertical)
        {
            switch (child->Get_Horizontal_Alignment(ctx))
            {
                case Alignment::Center:
                    x += (bounds.width - width) / 2.0f;
                    break;
                case Alignment::End:
                    x += (bounds.width - padding.x * 2 - width);
                    break;
                case Alignment::Stretch:
                    width = bounds.width - padding.x * 2;
                    break;
                case Alignment::Start:
                default:
                    break;
            }
        }
        else // Horizontal layout
        {
            switch (child->Get_Vertical_Alignment(ctx))
            {
                case Alignment::Center:
                    y += (bounds.height - height) / 2.0f;
                    break;
                case Alignment::End:
                    y += (bounds.height - padding.y * 2 - height);
                    break;
                case Alignment::Stretch:
                    height = bounds.height - padding.y * 2;
                    break;
                case Alignment::Start:
                default:
                    break;
            }
        }

        // Final bounds
        child->bounds = Rectangle{x, y, width, height};

        // Advance along main axis
        cursor += vertical ? height : width;
    }
}

void EUI_Container::Handle_Input(EUI_Context& ctx)
{
    if (!visible)
        return;

    for (EUI_Element* child : children)
    {
        if (child->visible)
        {
            child->Handle_Input(ctx);
        }
    }
}

void EUI_Container::Render(EUI_Context& ctx)
{
    if (!visible)
        return;

    const auto style = Get_Effective_Style(ctx);

    if (style.background_color)
    {
        DrawRectangleRec(bounds, *style.background_color);
    }

    if (style.border_thickness && style.border_thickness.value() > 0)
    {
        DrawRectangleLinesEx(bounds, *style.border_thickness, *style.border_color);
    }

    // Render children
    for (EUI_Element* child : children)
    {
        if (child->visible)
        {
            child->Render(ctx);
        }
    }
}
