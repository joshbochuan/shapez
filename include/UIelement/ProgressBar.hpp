//
// Created by joshb on 2026/4/12.
//

#ifndef SHAPEZ_PROGRESSBAR_HPP
#define SHAPEZ_PROGRESSBAR_HPP

#include "Util/Image.hpp"
#include "Opti/OptiObject.hpp"
#include "Text.hpp"

class ProgressBar: public OptiObject {
public:
    float progress=0, target=1;
    glm::vec2 dimension;
    std::shared_ptr<OptiObject> background, bar;
    std::shared_ptr<Util::Image> barImage, barCompleteImage;
    std::shared_ptr<Text> text;

    ProgressBar(std::shared_ptr<Util::Image> border);
    ProgressBar(glm::vec2 dimension);
    void Update();
    void SetBackground(std::shared_ptr<Util::Image> background);
    void SetBar(std::shared_ptr<Util::Image> bar);
    void SetBarComplete(std::shared_ptr<Util::Image> background);
    void SetBarZIndex(int zIndex);
};

#endif //SHAPEZ_PROGRESSBAR_HPP