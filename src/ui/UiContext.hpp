#pragma once

#include <memory>
#include <gtkmm.h>

#include "windows/MainWindow.hpp"

namespace ui {
class UiContext {
 private:
    std::unique_ptr<windows::MainWindow> mainWindow{nullptr};

 public:
    Glib::RefPtr<Gtk::Application> app{nullptr};

    int run(int argc, char** argv);

    void add_main_window();

    // void add_selector_window();
    // void remove_selector_window();
};
}  // namespace ui
