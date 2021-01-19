#include "MainWindow.hpp"

namespace ui::windows {
MainWindow::MainWindow() {
    prep_window();
}

void MainWindow::prep_window() {
    set_title("Basic application");
    set_default_size(200, 200);
}
}  // namespace ui::windows
