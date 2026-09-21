#include <iostream>
#include <string>

#include "Controller.hpp"
#include "Model.hpp"
#include "View.hpp"

int main() {
    Model model;
    View view;
    Controller controller(model, view);

    view.showMessage("Type 'help' for a list of commands.");

    std::string line;
    bool running = true;
    while (running) {
        view.showPrompt();
        if (!std::getline(std::cin, line)) {
            break;
        }
        running = controller.handleCommand(line);
    }

    return 0;
}
