#include "Controller.hpp"
#include "AsciiArt.hpp"

#include <sstream>
#include <charconv>

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

    if (command == "set_speed") {
        if (tokens.size() != 2) {
            view_.showMessage("Usage: set_speed <positive_int>");
            return true;
        }

        try {
            std::size_t pos = 0;
            int speed = std::stoi(tokens[1], &pos);

            // Rejects trailing characters such as "12abc"
            if (pos != tokens[1].size()) {
                view_.showMessage("Error: Speed contains non-numeric characters.");
                return true;
            }

            // Must be positive
            if (speed <= 0) {
                view_.showMessage("Error: Speed must be greater than 0.");
                return true;
            }

            model_.setSpeed(speed);
            view_.showMessage("Speed set to " + std::to_string(speed));
        } 
        catch (const std::invalid_argument&) {
            view_.showMessage("Error: Invalid number format for speed.");
        } 
        catch (const std::out_of_range&) {
            view_.showMessage("Error: Speed value is too large.");
        }

        return true;
    }

    if (command == "start_marquee") {
        view_.showMarquee(renderAsciiArt(model_.getText()), model_.getSpeed());
        return true;
    }

    if (command == "stop_marquee") {
        if (!view_.isMarqueeRunning()) {
            view_.showMessage("No marquee is currently running.");
        } else {
            view_.stopMarquee();
        }
        return true;
    }

    view_.showMessage("Unknown command: " + command + " (type 'help')");
    return true;
}
