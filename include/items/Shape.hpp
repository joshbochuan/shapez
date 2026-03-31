//
// Created by User on 2026/3/20.
//

#ifndef SHAPEZ_SHAPE_HPP
#define SHAPEZ_SHAPE_HPP
#include "Quad.hpp"
#include "Item.hpp"
#include "Util/GameObject.hpp"
#include <string>
#include "Util/Image.hpp"

class Shape: public Item {
private:
    std::string code;
public:
    static inline std::shared_ptr<Util::Image> shapeTexture; // dark circle below
    std::vector<std::shared_ptr<Quad>> quads;
    Shape(std::string code);
    void Update() override;
    std::string getCode() override;
    std::shared_ptr<Item> copy() override;
    bool operator==(std::shared_ptr<Item> rhs) override;
    void MachineItemZIndex(float index) override;
    void SetItemSize(glm::vec2 s) override;
};


#endif //SHAPEZ_SHAPE_HPP