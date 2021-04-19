#pragma once

#include <gtkmm-3.0/gtkmm.h>

namespace ui {
Glib::RefPtr<Gtk::CssProvider> get_css_provider();
}  // namespace ui