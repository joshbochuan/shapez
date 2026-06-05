//
// Created by joshb on 2026/3/21.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_TEXT_HPP
#define REPLACE_WITH_YOUR_PROJECT_NAME_TEXT_HPP
#include "Util/GameObject.hpp"
#include "Util/Color.hpp"
#include "Util/Text.hpp"
#include "../Opti/OptiObject.hpp"

class Text: public OptiObject {
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

    void AlignToLeft() {SetPivot({-0.5f * m_Text->GetSize().x, 0});}
    void AlignToRight() {SetPivot({0.5f * m_Text->GetSize().x, 0});}
    void SetText(const std::string &text) const {m_Text->SetText(text);}
};

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_TEXT_HPP