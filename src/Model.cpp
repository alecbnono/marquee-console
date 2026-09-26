#include "Model.hpp"

void Model::setText(const std::string& text) {
    text_ = text;
}

const std::string& Model::getText() const {
    return text_;
}

void Model::setSpeed(const int speed) {
    speed_ = speed;
} 

int Model::getSpeed() const {
    return speed_;
}