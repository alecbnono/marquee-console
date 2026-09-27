#include "View.hpp"

#include <iostream>

#include "AsciiArt.hpp"

namespace {
// Region boundary constants for split-screen console display.
// Top: Marquee Region (rows 1 to 10)
// Bottom: Console Region (rows 12+)
constexpr int kMarqueeStartRow = 1;
constexpr int kMarqueeMaxRows = 10;
constexpr int kConsoleStartRow = 12;
constexpr int kWindowWidth = 100;
}  // namespace

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
            showMessage("\nMarquee stopped.");
        } else {
            marqueeThread_.join();
        }
    }
}

void View::showMarquee(const std::vector<std::string>& text, int speed) {
    stopMarquee();

    stopRequested_ = false;
    
    // Pads all rows to the same width
    int rowWidth = 0;
    for (const auto& r : text) {
        if ((int)r.size() > rowWidth) rowWidth = (int)r.size();
    }

    // Pads each row to the same width so scrolling window is stable
    std::vector<std::string> paddedText = text;
    for (auto& r : paddedText) {
        while ((int)r.size() < rowWidth) r += ' ';
    }

    isRunning_ = true;
    marqueeThread_ = std::thread([this, paddedText, text, rowWidth, speed]() {
        // Offset scrolls from off-screen left to off-screen right
        for (int offset = -kWindowWidth; offset <= rowWidth + kWindowWidth; ++offset) {
            if (stopRequested_) break;

            // Save current cursor position in the console region
            std::cout << "\x1b[s";

            // Move cursor to row 1 and overwrite marquee region rows with spaces to clear remnants
            for (int r = 0; r < kMarqueeMaxRows; ++r) {
                std::cout << "\x1b[" << (kMarqueeStartRow + r) << ";1H";
                std::cout << std::string(kWindowWidth, ' ');
            }

            // Draw the new marquee frame within the Marquee Region
            for (int r = 0; r < (int)text.size() && r < kMarqueeMaxRows; ++r) {
                std::cout << "\x1b[" << (kMarqueeStartRow + r) << ";1H";

                std::string line;
                for (int c = 0; c < kWindowWidth; ++c) {
                    int src = offset + c;
                    if (src >= 0 && src < rowWidth) {
                        line += paddedText[r][src];
                    } else {
                        line += ' ';
                    }
                }
                std::cout << line;
            }

            // Restore the cursor back down to the prompt line in the Console Region
            std::cout << "\x1b[u" << std::flush;

            std::this_thread::sleep_for(std::chrono::milliseconds(speed));
        }

        // Restore cursor position on animation completion
        std::cout << "\x1b[u" << std::flush;
        isRunning_ = false;
    });
}
