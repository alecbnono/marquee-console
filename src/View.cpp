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
        "  set_text <text> save the text to use for the marquee\n"
        "  start_marquee   starts the marquee \"animation\"\n"
        "  help            show this message\n"
        "  exit            quit the program\n";
}

void View::showPrompt() const {
    std::cout << "> ";
}

void View::showMarquee(const std::vector<std::string>& text, int speed) const {
        
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

    const int ROW = 5;
    const int COL = 1;
    const int WINDOW = 100;     // visible columns

    // offset goes from negative to past the end
    for (int offset = -WINDOW; offset <= rowWidth + WINDOW; ++offset) {
        for (int r = 0; r < (int)text.size(); ++r) {
            std::cout << "\x1b[" << (ROW + r) << ";" << COL << "H";

            // Builds visible window: character at paddedText[r][offset + c] maps to screen column c
            std::string line;
            for (int c = 0; c < WINDOW; ++c) {
                int src = offset + c;
                if (src >= 0 && src < rowWidth) {
                    line += paddedText[r][src];  
                } else {
                    line += ' ';
                }
            }
            // Erase leftover characters from previous frame
            line.append(WINDOW, ' ');
            std::cout << line;
        }

        std::cout << "\x1b[" << (ROW + text.size() + 1) << ";" << COL << "H";
        std::cout << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(speed));
    }

    // Move cursor below the animation and print a newline
    std::cout << "\x1b[" << (ROW + text.size() + 2) << ";" << COL << "H";
    std::cout << "\n";
}
