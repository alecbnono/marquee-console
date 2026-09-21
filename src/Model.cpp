#include "Model.hpp"

void Model::setText(const std::string& text) {
    text_ = text;
}

const std::string& Model::getText() const {
    return text_;
}
