//
// Created by joshb on 2026/4/25.
//

#ifndef SHAPEZ_CHUNK_HPP
#define SHAPEZ_CHUNK_HPP
#include "Opti/OptiObject.hpp"
#include "items/Item.hpp"
#include "items/Shape.hpp"
#include "items/Color.hpp"

class Cell: public OptiObject {
public:
    inline static std::shared_ptr<OptiImage> cellTextureGray;
    inline static std::shared_ptr<OptiImage> cellTextureRed;
    inline static std::shared_ptr<OptiImage> cellTextureGreen;
    inline static std::shared_ptr<OptiImage> cellTextureBlue;

    int x=0, y=0; // 0 <= x < 16, 0 <= y < 16, fits inside a chunk
    std::shared_ptr<Item> item = nullptr;
    Cell(int x, int y, std::shared_ptr<Item> item);
    void Update();
};

class Chunk: public OptiObject, public std::enable_shared_from_this<Chunk> {
public:
    int x=0, y=0; // each chunk is 16x16
    std::shared_ptr<Item> item = nullptr;
    bool minePoints[16][16]; // true if that point can be mined
    std::vector<std::shared_ptr<Cell>> cells;

    Chunk(int x, int y);
    void Generate(int seed);
    void Update();
};

#endif //SHAPEZ_CHUNK_HPP