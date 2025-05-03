#include <raylib.h>
#include <string>

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
    Color background_color, text_color, border_color;
    float border_radius, border_thickness;
    Font font;
};

class EUI_Element
{
    Element_Type type;
    std::string id;

    Layout_Model layout;
    Vector2 min_size, max_size, preferred_size;
    Vector2 margin, padding;
    Vector4 bounds;

    bool visible, hovered, focused, active;

    EUI_Style style;
    std::string text;
    Image texture;
};

class EUI_Context
{
    EUI_Element* root;

    EUI_Input input;
};
