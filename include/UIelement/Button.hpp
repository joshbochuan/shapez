//
// Created by User on 2026/3/27.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_UI_HPP
#define REPLACE_WITH_YOUR_PROJECT_NAME_UI_HPP
#include "Util/GameObject.hpp"
#include "Text.hpp"
#include "Util/Image.hpp"
#include "../Opti/OptiObject.hpp"
#include "Opti/OptiImage.hpp"

class Button: public OptiObject {
public:
    std::shared_ptr<Text> text;
    std::shared_ptr<OptiImage> image, imagePressed;
    glm::vec2 dimension;
    bool isClicked = false;
    bool isHeld = false;
    bool isReleased = false;
    Button(glm::vec2 dimension, std::string path, std::string text, float size, Util::Color color);
    void Update();
};

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_UI_HPP
