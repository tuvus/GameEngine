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
  public:
    Vector2 mouse_position;
    bool left_mouse_pressed;
    bool right_mouse_pressed;
    bool left_mouse_down;
    bool right_mouse_down;
    int key_pressed;
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
    std::vector<EUI_Element*> layout_stack;

    EUI_Input input;

    EUI_Element* hovered = nullptr;
    EUI_Element* active = nullptr;
    EUI_Element* focused = nullptr;

    EUI_Style default_style;

    float global_scale = 1.0f;
    float dpi_factor = 1.0f;

    void Begin_Frame();
    void End_Frame();
    void Update_Input();
    void Perform_Layout();
    void Handle_Input();
    void Render();
};

class EUI_Element
{
  public:
    virtual ~EUI_Element() = default;

    std::string id;
    Element_Type type;
    Layout_Model layout;

    EUI_Element* parent = nullptr;
    std::vector<EUI_Element*> children;

    Rectangle bounds;
    Vector2 min_size, max_size, preferred_size;
    Vector2 margin, padding;

    bool visible = true;
    bool hovered = false;
    bool focused = false;
    bool active = false;

    EUI_Style style;

    Color Get_Background_Color(const EUI_Context& ctx) const;
    Color Get_Text_Color(const EUI_Context& ctx) const;
    Color Get_Border_Color(const EUI_Context& ctx) const;
    float Get_Border_Radius(const EUI_Context& ctx) const;
    float Get_Border_Thickness(const EUI_Context& ctx) const;
    Font Get_Font(const EUI_Context& ctx) const;
    EUI_Style Get_Effective_Style(const EUI_Context& ctx) const;

    virtual void Layout(EUI_Context& ctx) = 0;
    virtual void Handle_Input(EUI_Context& ctx) = 0;
    virtual void Render(EUI_Context& ctx) = 0;

    void add_child(EUI_Element* child);
};
