#include <optional>
#include <raylib.h>
#include <string>
#include <vector>

enum class Element_Type
{
    Container,
    Stack,
    Flex,
    Grid,
    Absolute,
    Overlay,
    Label,
    Button,
    Image,
    Spacer
};

enum class Layout_Model
{
    StackHorizontal,
    StackVertical,
    FlexHorizontal,
    FlexVertical,
    Grid,
    Absolute,
    Overlay
};

enum class Alignment
{
    Start,
    Center,
    End,
    Stretch
};

class EUI_Input;
class EUI_Style;
class EUI_Element;
class EUI_Context;

class EUI_Input
{
    Vector2 mouse_position;
    bool left_mouse_pressed;
    bool right_mouse_pressed;
    bool left_mouse_down;
    bool right_mouse_down;
    // add keyboard?
};

class EUI_Style
{
  public:
    std::optional<Color> background_color, text_color, border_color;
    std::optional<float> border_radius, border_thickness;
    std::optional<Font> font;
};

class EUI_Context
{
  public:
    EUI_Element* root;

    EUI_Input input;

    EUI_Style default_style;
};

class EUI_Element
{
    Element_Type type;
    std::string id;

    Layout_Model layout;
    Vector2 min_size, max_size, preferred_size;
    Vector2 margin, padding;
    Rectangle bounds;

    bool visible, hovered, focused, active;

    EUI_Element* parent;
    std::vector<EUI_Element*> children;

    EUI_Style style;
    std::string text;
    Image texture;

    Color Get_Background_Color(const EUI_Context& ctx) const
    {
        if (style.background_color)
            return *style.background_color;
        if (parent)
            return parent->Get_Background_Color(ctx);
        return *ctx.default_style.background_color;
    }
    Color Get_Text_Color(const EUI_Context& ctx) const
    {
        if (style.text_color)
            return *style.text_color;
        if (parent)
            return parent->Get_Text_Color(ctx);
        return *ctx.default_style.text_color;
    };
    Color Get_Border_Color(const EUI_Context& ctx) const
    {
        if (style.border_color)
            return *style.border_color;
        if (parent)
            return parent->Get_Border_Color(ctx);
        return *ctx.default_style.border_color;
    };
    float Get_Border_Radius(const EUI_Context& ctx) const
    {
        if (style.border_radius)
            return *style.border_radius;
        if (parent)
            return parent->Get_Border_Radius(ctx);
        return *ctx.default_style.border_radius;
    };
    float Get_Border_Thickness(const EUI_Context& ctx) const
    {
        if (style.border_thickness)
            return *style.border_thickness;
        if (parent)
            return parent->Get_Border_Thickness(ctx);
        return *ctx.default_style.border_thickness;
    };
    Font Get_Font(const EUI_Context& ctx) const
    {
        if (style.font)
            return *style.font;
        if (parent)
            return parent->Get_Font(ctx);
        return *ctx.default_style.font;
    };

    EUI_Style Get_Effective_Style(const EUI_Context& ctx)
    {
        return {
            .text_color = Get_Text_Color(ctx),
            .background_color = Get_Background_Color(ctx),
            .border_color = Get_Border_Color(ctx),
            .border_radius = Get_Border_Radius(ctx),
            .font = Get_Font(ctx),
        };
    }
};
