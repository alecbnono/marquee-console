#include "View.hpp"

#include <iostream>

#include "AsciiArt.hpp"

void View::showMessage(const std::string& message) const {
    std::cout << message << "\n";
}

void View::showAsciiArt(const std::string& text) const {
    for (const std::string& row : renderAsciiArt(text)) {
        std::cout << row << "\n";
    }
}

void View::showHelp() const {
    std::cout <<
        "Commands:\n"
        "  set_text <text>          save the text to use for the marquee\n"
        "  set_speed <positive_int> save the speed of marquee animation (in ms)\n"
        "  start_marquee            starts the marquee \"animation\"\n"
        "  stop_marquee             stops the marquee \"animation\"\n"
        "  help                     show this message\n"
        "  exit                     quit the program\n";
}

void View::showPrompt() const {
    std::cout << "> ";
}

View::~View() {
    stopMarquee();
}

bool View::isMarqueeRunning() const {
    return isRunning_.load();
}

void View::stopMarquee() {
    if (marqueeThread_.joinable()) {
        if (isRunning_.load()) {
            stopRequested_ = true;
            marqueeThread_.join();
            showMessage("Marquee stopped.");
        } else {
            marqueeThread_.join();
        }
    }
}

void View::showMarquee(const std::vector<std::string>& text, int speed) {
    stopMarquee();

    stopRequested_ = false;

    int rowWidth = 0;
    for (const auto& r : text) {
        if ((int)r.size() > rowWidth) rowWidth = (int)r.size();
    }

    std::vector<std::string> paddedText = text;
    for (auto& r : paddedText) {
        while ((int)r.size() < rowWidth) r += ' ';
    }

    isRunning_ = true;
    marqueeThread_ = std::thread([this, paddedText, text, rowWidth, speed]() {
        const int ROW = 5;
        const int COL = 1;
        const int WINDOW = 100;

        for (int offset = -WINDOW; offset <= rowWidth + WINDOW; ++offset) {
            if (stopRequested_) break;

            for (int r = 0; r < (int)text.size(); ++r) {
                std::cout << "\x1b[" << (ROW + r) << ";" << COL << "H";

                std::string line;
                for (int c = 0; c < WINDOW; ++c) {
                    int src = offset + c;
                    if (src >= 0 && src < rowWidth) {
                        line += paddedText[r][src];
                    } else {
                        line += ' ';
                    }
                }
                line.append(WINDOW, ' ');
                std::cout << line;
            }

            std::cout << "\x1b[" << (ROW + text.size() + 1) << ";" << COL << "H";
            std::cout << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(speed));
        }

        std::cout << "\x1b[" << (ROW + (int)text.size() + 2) << ";" << COL << "H";
        std::cout << "\n" << std::flush;
        isRunning_ = false;
    });
}

