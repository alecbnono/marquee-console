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
        "  help            show this message\n"
        "  exit            quit the program\n";
}

void View::showPrompt() const {
    std::cout << "> ";
}
