//
// Created by joshb on 2026/3/25.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_COLOR_HPP
#define REPLACE_WITH_YOUR_PROJECT_NAME_COLOR_HPP
#include "Item.hpp"
#include "Util/Image.hpp"

inline std::unordered_map<std::string, std::shared_ptr<Util::Image>> colorTextures;

class Color: public Item {
private:
    int color = 0; // right-most bit means r, g, b, OR-ing gives the mix result
public:
    Color(int color);
    Color(std::string code);
    void Update();
    std::string getCode() override;
    int getColor();
    std::shared_ptr<Item> copy() override;
    bool operator==(std::shared_ptr<Item> rhs) override;
    void MachineItemZIndex(float index) override;
    void SetItemSize(glm::vec2 s) override;
};

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_COLOR_HPP