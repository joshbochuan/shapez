//
// Created by joshb on 2026/3/21.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_TEXT_HPP
#define REPLACE_WITH_YOUR_PROJECT_NAME_TEXT_HPP
#include "Util/GameObject.hpp"
#include "Util/Color.hpp"
#include "Util/Text.hpp"
#include "ShapezObject.hpp"

class Text: public ShapezObject {
public:
    std::shared_ptr<Util::Text> m_Text;
    Text(std::string text, float size, Util::Color color) {
        m_Text = std::make_unique<Util::Text>(
            "../Resources/fonts/GameFont.ttf",
            size,
            text,
            color);
        SetDrawable(m_Text);
    };
};

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_TEXT_HPP