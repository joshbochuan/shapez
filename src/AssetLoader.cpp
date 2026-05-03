//
// Created by joshb on 2026/3/31.
//
#include "AssetLoader.hpp"

#include "Chunk.hpp"
#include "items/Shape.hpp"
#include "items/Color.hpp"
#include "buildings/Belt.hpp"
#include "buildings/Balancer.hpp"
#include "buildings/Cutter.hpp"
#include "buildings/Miner.hpp"
#include "buildings/Mixer.hpp"
#include "buildings/Painter.hpp"
#include "buildings/Rotator.hpp"
#include "buildings/Stacker.hpp"
#include "buildings/Trash.hpp"
#include "buildings/Tunnel.hpp"
#include "scenes/GameScene.hpp"
#include "scenes/TitleScene.hpp"
#include "scenes/UpgradeScene.hpp"

void AssetLoader::loadTextures() {
    Shape::shapeTexture = std::make_shared<OptiImage>("../Resources/shapes/shape.png");
    std::string quadChoice="CRSW", colorChoice="bcgpruwy", code;
    for (int i=0; i<4; i++) {for (int j=0; j<8; j++) {for (int k=0; k<4; k++) {
        code = quadChoice[i];
        code += colorChoice[j];
        code += std::to_string(k);
        Quad::quadTextures[code] = std::make_shared<OptiImage>("../Resources/shapes/" + code + ".png");
    }}}
    Color::colorTextures.clear();
    Color::colorTextures.push_back(std::make_shared<OptiImage>("../Resources/sprites/colors/uncolored.png"));
    Color::colorTextures.push_back(std::make_shared<OptiImage>("../Resources/sprites/colors/blue.png"));
    Color::colorTextures.push_back(std::make_shared<OptiImage>("../Resources/sprites/colors/green.png"));
    Color::colorTextures.push_back(std::make_shared<OptiImage>("../Resources/sprites/colors/cyan.png"));
    Color::colorTextures.push_back(std::make_shared<OptiImage>("../Resources/sprites/colors/red.png"));
    Color::colorTextures.push_back(std::make_shared<OptiImage>("../Resources/sprites/colors/purple.png"));
    Color::colorTextures.push_back(std::make_shared<OptiImage>("../Resources/sprites/colors/yellow.png"));
    Color::colorTextures.push_back(std::make_shared<OptiImage>("../Resources/sprites/colors/white.png"));
    Belt::beltForwardTexture.clear();
    Belt::beltLeftTexture.clear();
    Belt::beltRightTexture.clear();
    Belt::beltInTexture.clear();
    Belt::beltOutTexture.clear();
    for (int i=0; i<=13; i++) {
        Belt::beltForwardTexture.push_back(std::make_shared<OptiImage>(
            "../Resources/sprites/belt/built/forward_" + std::to_string(i) +".png"));
        Belt::beltLeftTexture.push_back(std::make_shared<OptiImage>(
            "../Resources/sprites/belt/built/left_" + std::to_string(i) +".png"));
        Belt::beltRightTexture.push_back(std::make_shared<OptiImage>(
            "../Resources/sprites/belt/built/right_" + std::to_string(i) +".png"));
        Belt::beltInTexture.push_back(std::make_shared<OptiImage>(
            "../Resources/sprites/belt/built/forward_" + std::to_string(i) + "_bottom.png"));
        Belt::beltOutTexture.push_back(std::make_shared<OptiImage>(
            "../Resources/sprites/belt/built/forward_" + std::to_string(i) + "_top.png"));
    }
    Splitter::splitterTexture.clear();
    Splitter::splitterTexture.push_back(std::make_shared<OptiImage>("../Resources/sprites/buildings/balancer-splitter.png"));
    Splitter::splitterTexture.push_back(std::make_shared<OptiImage>("../Resources/sprites/buildings/balancer-splitter-inverse.png"));
    Merger::mergerTexture.clear();
    Merger::mergerTexture.push_back(std::make_shared<OptiImage>("../Resources/sprites/buildings/balancer-merger.png"));
    Merger::mergerTexture.push_back(std::make_shared<OptiImage>("../Resources/sprites/buildings/balancer-merger-inverse.png"));
    Balancer::balancerTexture = std::make_shared<OptiImage>("../Resources/sprites/buildings/balancer.png");
    Cutter::cutterTexture = std::make_shared<OptiImage>("../Resources/sprites/buildings/cutter.png");
    Miner::minerTexture = std::make_shared<OptiImage>("../Resources/sprites/buildings/miner.png");
    Miner::chainedMinerTexture = std::make_shared<OptiImage>("../Resources/sprites/buildings/miner-chainable.png");
    Miner::minerCoverTextureGray = std::make_shared<OptiImage>("../Resources/1px/miner_cover_gray.png");
    Miner::minerCoverTextureRed = std::make_shared<OptiImage>("../Resources/1px/miner_cover_red.png");
    Miner::minerCoverTextureGreen = std::make_shared<OptiImage>("../Resources/1px/miner_cover_green.png");
    Miner::minerCoverTextureBlue = std::make_shared<OptiImage>("../Resources/1px/miner_cover_blue.png");
    Mixer::mixerTexture = std::make_shared<OptiImage>("../Resources/sprites/buildings/mixer.png");
    Rotator::rotatorCWTexture = std::make_shared<OptiImage>("../Resources/sprites/buildings/rotater.png");
    Rotator::rotator180Texture = std::make_shared<OptiImage>("../Resources/sprites/buildings/rotater-rotate180.png");
    Rotator::rotatorCCWTexture = std::make_shared<OptiImage>("../Resources/sprites/buildings/rotater-ccw.png");
    Stacker::stackerTexture = std::make_shared<OptiImage>("../Resources/sprites/buildings/stacker.png");
    Trash::trashTexture = std::make_shared<OptiImage>("../Resources/sprites/buildings/trash.png");
    Painter::painterTextures.clear();
    Painter::painterTextures.push_back(std::make_shared<OptiImage>("../Resources/sprites/buildings/painter.png"));
    Painter::painterTextures.push_back(std::make_shared<OptiImage>("../Resources/sprites/buildings/painter-mirrored.png"));
    Tunnel::tunnelInTextures.clear();
    Tunnel::tunnelOutTextures.clear();
    Tunnel::tunnelInTextures.push_back(std::make_shared<OptiImage>("../Resources/sprites/buildings/underground_belt_entry.png"));
    Tunnel::tunnelInTextures.push_back(std::make_shared<OptiImage>("../Resources/sprites/buildings/underground_belt_entry-tier2.png"));
    Tunnel::tunnelOutTextures.push_back(std::make_shared<OptiImage>("../Resources/sprites/buildings/underground_belt_exit.png"));
    Tunnel::tunnelOutTextures.push_back(std::make_shared<OptiImage>("../Resources/sprites/buildings/underground_belt_exit-tier2.png"));
    for (int i=1; i<=8; i++) {
        UpgradeBlob::tierBackgroundImages.push_back(std::make_shared<Util::Image>("../Resources/ui/blobs/upgradeTier" + std::to_string(i) + ".png"));
    }
    Cell::cellTextureGray = std::make_shared<OptiImage>("../Resources/1px/background_gray.png");
    Cell::cellTextureRed = std::make_shared<OptiImage>("../Resources/1px/background_red.png");
    Cell::cellTextureGreen = std::make_shared<OptiImage>("../Resources/1px/background_green.png");
    Cell::cellTextureBlue = std::make_shared<OptiImage>("../Resources/1px/background_blue.png");
}

void AssetLoader::loadAudio() {
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    GameScene::placeBeltSFX = std::make_shared<Util::SFX>("../Resources/sounds/sfx/place_belt.wav");
    GameScene::placeBuildingSFX = std::make_shared<Util::SFX>("../Resources/sounds/sfx/place_building.wav");
    GameScene::destroyBuildingSFX = std::make_shared<Util::SFX>("../Resources/sounds/sfx/destroy_building.wav");
    Button::buttonSFX = std::make_shared<Util::SFX>("../Resources/sounds/sfx/ui_click.wav");
    UpgradeBlob::upgradeSFX = std::make_shared<Util::SFX>("../Resources/sounds/sfx/unlock_upgrade.wav");

    TitleScene::titleBGM = std::make_shared<Util::BGM>("../Resources/sounds/music/menu.wav");
    GameScene::shapezBGM = std::make_shared<Util::BGM>("../Resources/sounds/music/theme-full.mp3");
}