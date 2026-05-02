//
// Created by joshb on 2026/5/2.
//

#ifndef SHAPEZ_TEXTINPUT_HPP
#define SHAPEZ_TEXTINPUT_HPP
#include "Button.hpp"
#include "Text.hpp"

class TextInput: public OptiObject {
public:
    TextInput(std::shared_ptr<Util::Image> box, int textSize, Util::Color color);
    bool selected = false;
    int textSize = 12;
    std::string text;
    std::shared_ptr<Button> background;
    std::shared_ptr<Text> textDisplay;
    std::shared_ptr<OptiObject> pointer;
    void Update();
};

#endif //SHAPEZ_TEXTINPUT_HPP