#include <functional>
#include <optional>
#include <raylib.h>
#include <string>
#include <vector>

// TOP, RIGHT, BOTTOM, LEFT
typedef struct Sides {
  public:
    float top, right, bottom, left;
} Sides;

enum class Layout_Model { Horizontal, Vertical };

enum class Alignment { Start, Center, End, Stretch };

class EUI_Input;
class EUI_Style;
class EUI_Element;
class EUI_Context;

class EUI_Input {
  public:
    Vector2 mouse_position;
    bool left_mouse_pressed;
    bool right_mouse_pressed;
    bool left_mouse_down;
    bool right_mouse_down;
    int key_pressed;
};

class EUI_Style {
  public:
    std::optional<Color> background_color;
    std::optional<Color> text_color;

    std::optional<Color> border_color;
    std::optional<float> border_radius;
    std::optional<float> border_thickness;

    std::optional<Font> font;
    std::optional<float> font_size;
    std::optional<float> font_spacing;

    std::optional<Alignment> horizontal_alignment;
    std::optional<Alignment> vertical_alignment;
    std::optional<Alignment> text_horizontal_alignment;
    std::optional<Alignment> text_vertical_alignment;
};

class EUI_Context {
  public:
    EUI_Context();

    EUI_Element* root;

    EUI_Input input;

    EUI_Element* hovered = nullptr;
    EUI_Element* active = nullptr;
    EUI_Element* focused = nullptr;

    EUI_Style default_style = {
        /*.background_color = RAYWHITE,*/
        .text_color = BLACK,

        .border_color = BLACK,
        .border_radius = 0,
        .border_thickness = 0,

        .font = GetFontDefault(),
        .font_size = 15,
        .font_spacing = 1,

        .horizontal_alignment = Alignment::Start,
        .vertical_alignment = Alignment::Start,
        .text_horizontal_alignment = Alignment::Start,
        .text_vertical_alignment = Alignment::Start,
    };

    float global_scale = 1.0f;
    float dpi_factor = 1.0f;

    void Begin_Frame();
    void End_Frame();
    void Update_Input();
    void Perform_Layout();
    void Handle_Input();
    void Render();
};

class EUI_Element {
  public:
    virtual ~EUI_Element() = default;

    std::string id;

    EUI_Element* parent = nullptr;

    Vector2 pos, dim = {0};
    Vector2 min_size, max_size, preferred_size;
    Sides margin, padding;

    bool is_visible = true;
    bool is_hovered = false;
    bool is_focused = false;
    bool is_active = false;

    EUI_Style style;

    Color Get_Background_Color(const EUI_Context& ctx) const;
    Color Get_Text_Color(const EUI_Context& ctx) const;
    Color Get_Border_Color(const EUI_Context& ctx) const;
    float Get_Border_Radius(const EUI_Context& ctx) const;
    float Get_Border_Thickness(const EUI_Context& ctx) const;
    Font Get_Font(const EUI_Context& ctx) const;
    float Get_Font_Size(const EUI_Context& ctx) const;
    float Get_Font_Spacing(const EUI_Context& ctx) const;
    Alignment Get_Horizontal_Alignment(const EUI_Context& ctx) const;
    Alignment Get_Vertical_Alignment(const EUI_Context& ctx) const;
    Alignment Get_Text_Horizontal_Alignment(const EUI_Context& ctx) const;
    Alignment Get_Text_Vertical_Alignment(const EUI_Context& ctx) const;
    EUI_Style Get_Effective_Style(const EUI_Context& ctx) const;

    virtual void Layout(EUI_Context& ctx) = 0;
    virtual void Handle_Input(EUI_Context& ctx) = 0;
    virtual void Render(EUI_Context& ctx) = 0;

    virtual bool Is_Container() const { return false; };
};

class EUI_Container : public EUI_Element {
  protected:
    EUI_Container(Layout_Model lm) : layout_model(lm) {}
    std::vector<EUI_Element*> children;

  public:
    Layout_Model layout_model;

    float gap = 0;

    std::vector<EUI_Element*>& Get_Children() const;
    void Add_Child(EUI_Element* child);

    virtual void Layout(EUI_Context& ctx) override = 0;
    void Handle_Input(EUI_Context& ctx) override;
    void Render(EUI_Context& ctx) override;

    bool Is_Container() const override { return true; }
};

class EUI_VBox : public EUI_Container {
  public:
    EUI_VBox() : EUI_Container(Layout_Model::Vertical) {}

    void Layout(EUI_Context& ctx) override;
};

class EUI_HBox : public EUI_Container {
  public:
    EUI_HBox() : EUI_Container(Layout_Model::Horizontal) {}

    void Layout(EUI_Context& ctx) override;
};

class EUI_Text : public EUI_Element {
  public:
    EUI_Text(const std::string& text);

    std::string text;
    Vector2 text_pos;

    void Layout(EUI_Context& ctx) override;
    void Handle_Input(EUI_Context& ctx) override;
    void Render(EUI_Context& ctx) override;
};

class EUI_Button : public EUI_Text {
  public:
    EUI_Button(const std::string& text, std::function<void()> on_click);

    std::function<void()> on_click;

    void Handle_Input(EUI_Context& ctx) override;
};
