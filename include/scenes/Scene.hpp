//
// Created by joshb on 2026/3/31.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_SCENE_HPP
#define REPLACE_WITH_YOUR_PROJECT_NAME_SCENE_HPP
#include <memory>

class Scene: public std::enable_shared_from_this<Scene> {
public:
    virtual std::shared_ptr<Scene> Update() = 0; // returns the scene it's going to go to
};

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_SCENE_HPP