#pragma once

#include <string>

// Holds application state. No knowledge of input parsing or output formatting.
class Model {
public:
    void setText(const std::string& text);
    const std::string& getText() const;
    void setSpeed(const int speed);
    int getSpeed() const;

private:
    std::string text_ = "CSOPESY";
    int speed_ = 50;
};
