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

class MemoryArena
{
    uint8_t* base = nullptr; // Start of memory block
    size_t capacity = 0;     // Total size
    size_t offset = 0;       // How much is currently used

  public:
    MemoryArena(size_t size); // Allocate block
    ~MemoryArena();           // Free block

    void reset(); // Clear all allocations

    template <typename T>
    T* alloc(); // Allocate one object

    template <typename T>
    T* alloc_array(size_t count); // Allocate an array
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

    void Begin_Frame();
    void BeginVBox(std::string);
    void EndVBox();
    void Render();
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

    Color Get_Background_Color(const EUI_Context& ctx) const;
    Color Get_Text_Color(const EUI_Context& ctx) const;
    Color Get_Border_Color(const EUI_Context& ctx) const;
    float Get_Border_Radius(const EUI_Context& ctx) const;
    float Get_Border_Thickness(const EUI_Context& ctx) const;
    Font Get_Font(const EUI_Context& ctx) const;

    EUI_Style Get_Effective_Style(const EUI_Context& ctx);

    void Render();
};
