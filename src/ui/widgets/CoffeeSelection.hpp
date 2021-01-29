#pragma once

#include <gtkmm.h>
#include <gtkmm/flowbox.h>
#include <gtkmm/flowboxchild.h>

namespace ui::widgets {
class CoffeeSelection : public Gtk::FlowBox {
 public:
    CoffeeSelection();

 private:
    void prep_widget();

    //-----------------------------Events:-----------------------------
    static Gtk::FlowBoxChild& generate_button(const std::string& name, const std::string& cssClass, const Glib::RefPtr<Gtk::CssProvider>& cssProvider);
};
}  // namespace ui::widgets
