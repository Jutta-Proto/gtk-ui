#pragma once

#include <gtkmm.h>

namespace ui::windows {
class MainWindow : public Gtk::Window {
 private:
    Gtk::MenuButton* viewMoreBtn{nullptr};

 public:
    MainWindow();

 private:
    void prep_window();
    static void prep_overview(Gtk::Stack* stack);
    static void prep_advanced(Gtk::Stack* stack);

    //-----------------------------Events:-----------------------------
    void on_inspector_clicked();
};
}  // namespace ui::windows
