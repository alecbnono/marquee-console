#include "Controller.hpp"

#include <sstream>

Controller::Controller(Model& model, View& view) : model_(model), view_(view) {}

std::vector<std::string> Controller::tokenize(const std::string& line) {
    std::vector<std::string> tokens;
    std::istringstream stream(line);
    std::string token;
    while (stream >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

bool Controller::handleCommand(const std::string& line) {
    std::vector<std::string> tokens = tokenize(line);

    if (tokens.empty()) {
        return true;
    }

    const std::string& command = tokens[0];

    if (command == "exit") {
        return false;
    }

    if (command == "help") {
        view_.showHelp();
        return true;
    }

    if (command == "set_text") {
        if (tokens.size() < 2) {
            view_.showMessage("Usage: set_text <text>");
            return true;
        }
        std::string text = tokens[1];
        for (std::size_t i = 2; i < tokens.size(); ++i) {
            text += " " + tokens[i];
        }
        model_.setText(text);
        view_.showMessage("Text saved for marquee: " + text);
        return true;
    }

    view_.showMessage("Unknown command: " + command + " (type 'help')");
    return true;
}
