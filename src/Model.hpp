#pragma once

#include <string>

// Holds application state. No knowledge of input parsing or output formatting.
class Model {
public:
    void setText(const std::string& text);
    const std::string& getText() const;

private:
    std::string text_;
};
