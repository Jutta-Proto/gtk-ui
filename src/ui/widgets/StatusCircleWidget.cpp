#include "StatusCircleWidget.hpp"

namespace ui::widgets {
StatusCircleWidget::StatusCircleWidget() {
    prep_widget();
    this->signal_draw().connect(sigc::mem_fun(this, &StatusCircleWidget::on_draw_handler));
}

void StatusCircleWidget::prep_widget() {
    // Don't catch input. The widget has to be realized for this:
    signal_realize().connect([&] {
        get_window()->set_pass_through(true);
    });
}

void StatusCircleWidget::set_color(Gdk::RGBA&& color) {
    this->color = std::move(color);
}

//-----------------------------Events:-----------------------------
// NOLINTNEXTLINE (readability-convert-member-functions-to-static)
bool StatusCircleWidget::on_draw_handler(const Cairo::RefPtr<::Cairo::Context>& ctx) {
    ctx->save();
    ctx->arc(4, 4, 4, 0.0, 2.0 * M_PI);  // full circle
    ctx->set_source_rgba(color.get_red(), color.get_green(), color.get_blue(), color.get_alpha());  // partially translucent
    ctx->fill_preserve();
    ctx->restore();  // back to opaque black
    ctx->set_line_width(1);
    ctx->stroke();
    ctx->save();
    ctx->restore();
    return false;
}
}  // namespace ui::widgets
