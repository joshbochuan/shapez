//
// Created by User on 2026/3/27.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_UI_HPP
#define REPLACE_WITH_YOUR_PROJECT_NAME_UI_HPP
#include "Util/GameObject.hpp"
#include "Text.hpp"
#include "Util/Image.hpp"

class UIButton: public Util::GameObject {
public:
    std::shared_ptr<Text> text;
    std::shared_ptr<Util::Image> image, imagePressed;
    glm::vec2 dimension;
    bool isClicked = false;
    bool isHeld = false;
    bool isReleased = false;
    UIButton(glm::vec2 dimension, std::string path, std::string text, float size, Util::Color color);
    void Update();
};

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_UI_HPP
