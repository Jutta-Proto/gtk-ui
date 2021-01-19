#include "UiContext.hpp"

namespace ui {
int UiContext::run(int argc, char** argv) {
    // Create the main GTK application:
    app = Gtk::Application::create("org.jutta");

    app->signal_startup().connect([&] {
        add_main_window();
    });

    // The app will return once execution finished:
    return app->run(argc, argv);
}

void UiContext::add_main_window() {
    if (!mainWindow) {
        mainWindow = std::make_unique<windows::MainWindow>();
    }
    app->add_window(*mainWindow);
    mainWindow->show();
}

/*void UiContext::remove_selector_window() {
    if (sCtx) {
        Gtk::Window* window = sCtx->window.get_window();
        window->close();
        app->remove_window(*window);
    }
}*/

}  // namespace ui
