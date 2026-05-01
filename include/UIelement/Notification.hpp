//
// Created by joshb on 2026/5/1.
//

#ifndef SHAPEZ_NOTIFICATION_HPP
#define SHAPEZ_NOTIFICATION_HPP
#include "Opti/OptiObject.hpp"
#include "Util/Image.hpp"
#include "Text.hpp"
#include "config.hpp"

class Notification: public OptiObject {
public:
    Notification(std::string text, std::shared_ptr<Util::Image> symbol);
    void Update();

    std::shared_ptr<Text> text;
    std::shared_ptr<OptiObject> symbol;
    int frameToLive = FPS_CAP * 4;
};

#endif //SHAPEZ_NOTIFICATION_HPP