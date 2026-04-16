//
// Created by joshb on 2026/4/12.
//
#include "UIelement/ProgressBar.hpp"
#include "World.hpp"

using namespace World;

ProgressBar::ProgressBar(glm::vec2 dimension) {
    this->dimension = dimension;
    this->background = std::make_shared<OptiObject>();
    this->bar = std::make_shared<OptiObject>();
    this->bar->SetPivot({-0.5, 0});
    AddChild(background);
    AddChild(bar);
}

ProgressBar::ProgressBar(std::shared_ptr<Util::Image> border) {
    this->dimension = border->GetSize();
    this->background = std::make_shared<OptiObject>();
    this->bar = std::make_shared<OptiObject>();
    this->SetDrawable(border);
    this->bar->SetPivot({-0.5, 0});
    AddChild(background);
    AddChild(bar);
}

void ProgressBar::Update() {
    float percentage = progress / target;
    if (percentage < 0) {percentage = 0;}
    if (percentage >= 1) {
        percentage = 1;
        bar->SetDrawable(barCompleteImage);
    }
    else {
        bar->SetDrawable(barImage);
    }
    bar->m_Transform.scale = {m_Transform.scale.x * dimension.x * percentage, m_Transform.scale.y * dimension.y};
    bar->m_Transform.translation = {m_Transform.translation.x - (m_Transform.scale.x * dimension.x * 0.5), m_Transform.translation.y};
    background->m_Transform.translation = m_Transform.translation;
    background->m_Transform.scale = {m_Transform.scale.x * dimension.x, m_Transform.scale.y * dimension.y};
    if (text != nullptr) {
        text->m_Transform = m_Transform;
    }
}

void ProgressBar::SetBackground(std::shared_ptr<Util::Image> background) {
    this->background->SetDrawable(background);
}

void ProgressBar::SetBar(std::shared_ptr<Util::Image> bar) {barImage = bar;}

void ProgressBar::SetBarComplete(std::shared_ptr<Util::Image> barComplete) {barCompleteImage = barComplete;}

void ProgressBar::SetBarZIndex(int zIndex) {
    background->SetZIndex(zIndex);
    bar->SetZIndex(0.5f+zIndex);
    SetZIndex(zIndex+1);
}
