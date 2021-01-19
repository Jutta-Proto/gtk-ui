#pragma once

#include <gtkmm.h>

namespace ui::windows {
class MainWindow : public Gtk::Window {
 public:
    MainWindow();

 private:
    void prep_window();
};
}  // namespace ui::windows
