#pragma once

#include <gtkmm.h>
#include <gtkmm/frame.h>

namespace ui::widgets {
class CustomCoffeeWidget : public Gtk::Box {
 public:
    CustomCoffeeWidget();

 private:
    void prep_widget();
    static Gtk::FlowBoxChild& generate_button(const std::string& name, const std::string& cssClass, const Glib::RefPtr<Gtk::CssProvider>& cssProvider);

    //-----------------------------Events:-----------------------------
};
}  // namespace ui::widgets
