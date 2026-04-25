//
// Created by joshb on 2026/4/25.
//
#include "Chunk.hpp"

#include <iostream>
#include <cmath>
#include "World.hpp"
#include "items/Shape.hpp"
using namespace World;

Cell::Cell(int x, int y, std::shared_ptr<Item> item) {
    this->x = x;
    this->y = y;
    this->item = item;
    this->item->MachineItemZIndex(1); // same as grid
    AddChild(this->item);
    SetZIndex(4);
    if (item == nullptr) {return;}
    if (item->getType() == ItemType::SHAPE) {
        SetDrawable(cellTextureGray);
        return;
    }
    if (item->getType() != ItemType::COLOR) {return;}
    switch (std::dynamic_pointer_cast<Color>(item)->getColor()) {
        case 1: SetDrawable(cellTextureBlue); break;
        case 2: SetDrawable(cellTextureGreen); break;
        case 4: SetDrawable(cellTextureRed); break;
        default: SetDrawable(cellTextureGray); break;
    }
}

void Cell::Update() {
    m_Visible = ((std::abs(m_Transform.translation.x)-cam.scale.x*96 < WINDOW_WIDTH>>1)
        && (std::abs(m_Transform.translation.y)-cam.scale.y*96 < WINDOW_HEIGHT>>1));
    item->m_Transform.translation = m_Transform.translation;
    item->SetItemSize(cam.scale);
    item->Update();
}

Chunk::Chunk(int x, int y) {
    this->x = x;
    this->y = y;
    for (int i=0; i<16; i++) {for (int j=0; j<16; j++) {minePoints[i][j] = false;}}
    Generate(SEED);
    if (item != nullptr) {item->SetVisible(false);}
    for (int i=0; i<16; i++) {for (int j=0; j<16; j++) {
        if (!minePoints[i][j]) {continue;}
        cells.push_back(std::make_shared<Cell>(i, j, item->copy()));
        AddChild(cells.back());
    }}
}

int pseudo_random(int seed, int x, int y) {
    int n = seed;

    // Mix in x and y
    n ^= x * 0x9E3779B9;
    n ^= y * 0x85EBCA6B;

    // Bit mixing (hash-style scrambling)
    n ^= (n >> 16);
    n *= 0x7FEB352D;
    n ^= (n >> 15);
    n *= 0x846CA68B;
    n ^= (n >> 16);

    return n;
}

std::shared_ptr<Shape> GenerateRandomMapShape(int seed, int x, int y) {
    // generates shapes
    // no complete windmills
    int val = pseudo_random(seed, x, y);
    float distance = std::sqrt(static_cast<float>(x*x+y*y));
    int maxShapeAllowed = 1 + distance/10;
    if (maxShapeAllowed < 1) {maxShapeAllowed = 1;}
    if (maxShapeAllowed > 4) {maxShapeAllowed = 4;}

    std::vector<std::string> combinations = {
        "CRSW", "CRWS", "CSRW", "CSWR", "CWRS", "CWSR",
        "RCSW", "RCWS", "RSCW", "RSWC", "RWCS", "RWSC",
        "SCRW", "SCWR", "SRCW", "SRWC", "SWCR", "SWRC",
        "WCRS", "WCSR", "WRCS", "WRSC", "WSCR", "WSRC"
    };
    std::string deck = combinations[val%24];
    int q1Idx = val % maxShapeAllowed;
    int q2Idx = (val >> 2) % maxShapeAllowed;
    int q3Idx = (val >> 4) % maxShapeAllowed;
    int q4Idx = (val >> 6) % maxShapeAllowed;
    std::string code;
    code += deck[q1Idx];
    code += "u";
    code += deck[q2Idx];
    code += "u";
    code += deck[q3Idx];
    code += "u";
    code += deck[q4Idx];
    code += "u";
    if (code == "WuWuWuWu") {code = "CuCuCuRu";}

    return std::make_shared<Shape>(code);
}

// Smooth interpolation
float lerp(float a, float b, float t) {
    return a + t * (b - a);
}

// Smoothstep for nicer transitions
float smoothstep(float t) {
    return t * t * (3 - 2 * t);
}

void Chunk::Generate(int seed) {
    std::cout << "generating chunk" << std::endl;
    // decides whether this chunk will generate anything
    double val = static_cast<double>(pseudo_random(seed, x, y)) / 2147483647.0;
    if (val < 0) {val = -val;}
    // always put a circle and a square at 0, 0 and -1, -1
    if ((x == 0) && (y == 0)) {item = std::make_shared<Shape>("CuCuCuCu");}
    else if ((x == -1) && (y == -1)) {item = std::make_shared<Shape>("RuRuRuRu");}
    else if (val < 0.75) {item = nullptr;}
    else if (val < 0.88) {item = GenerateRandomMapShape(seed, x, y);}
    else if (val < 0.92) {item = std::make_shared<Color>(4);}
    else if (val < 0.96) {item = std::make_shared<Color>(2);}
    else {item = std::make_shared<Color>(1);}

    if (item == nullptr) {return;}

    glm::vec2 bl, br, tl, tr;
    bl.x = std::cos(pseudo_random(SEED, x, y));
    bl.y = std::sin(pseudo_random(SEED, x, y));
    br.x = std::cos(pseudo_random(SEED, x+1, y));
    br.y = std::sin(pseudo_random(SEED, x+1, y));
    tl.x = std::cos(pseudo_random(SEED, x, y+1));
    tl.y = std::sin(pseudo_random(SEED, x, y+1));
    tr.x = std::cos(pseudo_random(SEED, x+1, y+1));
    tr.y = std::sin(pseudo_random(SEED, x+1, y+1));

    // tune those
    float variance = 1;
    float exponent = 1;

    float distance = std::sqrt(static_cast<float>(x*x+y*y));
    float size = 0;
    if (distance < 40) {size = 1.4 - 0.01 * distance;}
    else {size = 1.0;}

    float height = 0;
    float blVal=0, brVal=0, tlVal=0, trVal=0;
    float bottomVal=0, topVal=0;
    for (int i=0; i<16; i++) {
        for (int j=0; j<16; j++) {
            blVal = glm::length(bl * glm::vec2({0.0625f * i, 0.0625f * j}));
            brVal = glm::length(br * glm::vec2({1-0.0625f * i, 0.0625f * j}));
            tlVal = glm::length(tl * glm::vec2({0.0625f * i, 1 - 0.0625f * j}));
            trVal = glm::length(tr * glm::vec2({1 - 0.0625f * i, 1 - 0.0625f * j}));

            bottomVal = lerp(blVal, brVal, smoothstep(0.0625f * i));
            topVal = lerp(tlVal, trVal, smoothstep(0.0625f * i));

            height = 0;
            height += std::pow(std::sin(M_PI*0.0625f*i), exponent);
            height += std::pow(std::sin(M_PI*0.0625f*j), exponent);
            height *= 0.5f;
            height += lerp(bottomVal, topVal, smoothstep(0.0625f * j)) * variance;
            minePoints[i][j] = height >= size;
        }
    }
}

void Chunk::Update() {
    this->m_Transform.translation.x = ((192.0*(8+(x<<4))) - cam.translation.x) * cam.scale.x;
    this->m_Transform.translation.y = ((192.0*(8+(y<<4))) - cam.translation.y) * cam.scale.y;
    if (std::abs(m_Transform.translation.x) >= 0.5f * WINDOW_WIDTH + 12288.0f * cam.scale.x
        || std::abs(m_Transform.translation.y) >= 0.5f * WINDOW_HEIGHT + 12288.0f * cam.scale.y) {
        chunks.erase({x, y});
        m_Root.RemoveChild(shared_from_this());
        return;
    }
    this->m_Transform.scale.x = cam.scale.x;
    this->m_Transform.scale.y = cam.scale.y;
    m_Visible = ((item != nullptr)
                && std::abs(m_Transform.translation.x) <= 0.5f * WINDOW_WIDTH + 1536.0f * cam.scale.x
                && std::abs(m_Transform.translation.y) <= 0.5f * WINDOW_HEIGHT + 1536.0f * cam.scale.y);
    for (auto& cell: cells) {
        cell->m_Transform.translation.x = m_Transform.translation.x + (cam.scale.x * (cell->x-7.5f) * 192.0f);
        cell->m_Transform.translation.y = m_Transform.translation.y + (cam.scale.y * (cell->y-7.5f) * 192.0f);
        cell->m_Transform.scale = cam.scale * 192.0f;
        cell->Update();
    }
}
