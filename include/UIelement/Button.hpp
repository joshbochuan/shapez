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
#include "Util/Input.hpp"

class Button: public OptiObject {
public:
    std::shared_ptr<Text> text;
    std::shared_ptr<OptiObject> image, lockedImage;
    std::shared_ptr<Util::Image> idleImage;
    std::shared_ptr<Util::Image> hoveredImage;
    std::shared_ptr<Util::Image> heldImage;
    std::shared_ptr<Util::Image> selectedImage;
    std::vector<Util::Keycode> keys; // other keys that can be used to do stuff with the button
    bool locked = false; // locked in idle state, no animation or responses

    float imageScale = 1, lockedImageScale = 1;

    float idleSpeed = 1;
    float hoverSpeed = 1;
    float heldSpeed = 1;
    float selectSpeed = 1;

    float idleScale = 1;
    float hoverScale = 1;
    float heldScale = 1;
    float selectScale = 1;

    glm::vec2 dimension;

    bool clicked = false;
    bool held = false;
    bool released = false;
    bool hovered = false;
    bool selected = false;
    float scaleState = 1;

    Button(std::shared_ptr<Util::Image> idleImage);
    void Update();
    void SetImage(std::shared_ptr<Util::Image> image);
};

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_UI_HPP
