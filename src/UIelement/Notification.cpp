//
// Created by joshb on 2026/5/1.
//
#include "UIelement/Notification.hpp"
#include "Util/Image.hpp"
#include "World.hpp"
using namespace World;

Notification::Notification(std::string text, std::shared_ptr<Util::Image> symbol) {
    SetDrawable(std::make_shared<Util::Image>("../Resources/ui/blobs/notification.png"));
    SetPivot({m_Drawable->GetSize().x * 0.5f, 0});
    SetZIndex(98);
    m_Transform.scale = {windowPercent, windowPercent};
    m_Transform.translation.x = -25.0f * windowPercent + (WINDOW_WIDTH >> 1);
    m_Transform.translation.y = 200.0f * windowPercent - (WINDOW_HEIGHT >> 1);
    this->text = std::make_shared<Text>(text, 32, Util::Color::FromRGB(255, 255, 255));
    this->text->SetPivot({-0.5f * this->text->m_Text->GetSize().x, -0.05f * this->text->m_Text->GetSize().y});
    this->text->m_Transform.translation.x = m_Transform.translation.x - m_Transform.scale.x * 450.0f;
    this->text->m_Transform.translation.y = m_Transform.translation.y;
    this->text->m_Transform.scale = m_Transform.scale;
    this->text->SetZIndex(99);
    AddChild(this->text);
    if (symbol != nullptr) {
        this->symbol = std::make_shared<OptiObject>();
        this->symbol->SetDrawable(symbol);
        this->symbol->m_Transform.translation.x = m_Transform.translation.x - m_Transform.scale.x * 500.0f;
        this->symbol->m_Transform.translation.y = m_Transform.translation.y;
        this->symbol->m_Transform.scale = m_Transform.scale * 0.5f;
        this->symbol->SetZIndex(99);
        AddChild(this->symbol);
    }
}

void Notification::Update() {
    frameToLive--;
    if (static_cast<float>(frameToLive) < 0.5f * FPS_CAP) {
        float t = static_cast<float>(frameToLive) / (0.5f * FPS_CAP);
        t = 1-t;
        m_Transform.scale.x = (-2.0f*t*t + t + 1) * windowPercent;
        m_Transform.scale.y = m_Transform.scale.x;
    }
    this->text->m_Transform.translation.x = m_Transform.translation.x - m_Transform.scale.x * 450.0f;
    this->text->m_Transform.translation.y = m_Transform.translation.y;
    this->text->m_Transform.scale = m_Transform.scale;
    if (symbol != nullptr) {
        this->symbol->m_Transform.translation.x = m_Transform.translation.x - m_Transform.scale.x * 500.0f;
        this->symbol->m_Transform.translation.y = m_Transform.translation.y;
        this->symbol->m_Transform.scale = m_Transform.scale * 0.5f;
    }
}
