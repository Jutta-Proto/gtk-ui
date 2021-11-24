#pragma once

#include <gdkmm/rgba.h>
#include <gtkmm.h>

namespace ui::widgets {
class StatusCircleWidget : public Gtk::DrawingArea {
 private:
    Gdk::RGBA color{};

 public:
    StatusCircleWidget();

    void set_color(Gdk::RGBA&& color);

 private:
    void prep_widget();

    //-----------------------------Events:-----------------------------
    bool on_draw_handler(const Cairo::RefPtr<::Cairo::Context>& ctx);
};
}  // namespace ui::widgets
