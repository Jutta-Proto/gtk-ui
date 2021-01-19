#include "logger/Logger.hpp"

int main(int /**argc**/, char** /**argv**/) {
    logger::setup_logger(spdlog::level::debug);
    SPDLOG_INFO("Launching Version: {} {}", JUTTA_UI_VERSION, JUTTA_UI_VERSION_NAME);
    return 0;
}
