#pragma once

#include "backend/NfcDetection.hpp"
#include <memory>
#include <string>
#include <gtkmm.h>
#include <gtkmm/button.h>
#include <gtkmm/frame.h>
#include <gtkmm/infobar.h>

namespace ui::widgets {
class NfcCardReaderWidget : public Gtk::Frame {
 private:
    Gtk::Button* cancelBtn{nullptr};
    Gtk::Spinner* runningSpinner{nullptr};
    Gtk::InfoBar* errorBar{nullptr};
    Gtk::Box* errorBarBox{nullptr};
    Gtk::Label* errorLabel{nullptr};
    Glib::RefPtr<Gdk::Pixbuf> nfcImageBuf{nullptr};

    std::unique_ptr<backend::NfcDetection> detection{nullptr};

    using type_signal_detection_successfull = sigc::signal<void, std::string>;
    using type_signal_detection_canceled = sigc::signal<void>;
    type_signal_detection_successfull m_signal_detection_successfull;
    type_signal_detection_canceled m_signal_detection_canceled;

 public:
    NfcCardReaderWidget();

    type_signal_detection_successfull signal_detection_successfull();
    type_signal_detection_canceled signal_detection_canceled();

    void start_detecting();
    void stop_detecting();

 private:
    void prep_widget();
    void prep_error_bar();

    //-----------------------------Events:-----------------------------
    void on_cancel_btn_click();
    void on_detection_state_changed(const backend::NfcDetection::NfcDetectionState& state);
    void on_error_bar_response(int response);
};
}  // namespace ui::widgets
