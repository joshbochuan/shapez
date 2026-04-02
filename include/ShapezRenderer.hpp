//
// Created by joshb on 2026/4/1.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_SHAPEZRENDERER_HPP
#define REPLACE_WITH_YOUR_PROJECT_NAME_SHAPEZRENDERER_HPP

#include <vector>
#include <memory>
#include "Util/GameObject.hpp"
#include "ShapezObject.hpp"

class App;

/**
 * @class ShapezRenderer
 * @brief A class optimized for calling shapez object's Draw()
 * @see Util::GameObject
 */
class ShapezRenderer final {
public:
    /**
     * @brief Parameterized constructor.
     *`
     *
     * @param children The GameObject needing to be managed by Renderer.
     */
    ShapezRenderer(const std::vector<std::shared_ptr<ShapezObject>> &children = {});

    /**
     * @brief Add a child to Renderer.
     *
     * @param child The GameObject needing to be managed by Renderer.
     */
    void AddChild(const std::shared_ptr<ShapezObject> &child);

    /**
     * @brief Add children to Renderer.
     *
     * @param children The GameObjects needing to be managed by Renderer.
     */
    void AddChildren(const std::vector<std::shared_ptr<ShapezObject>> &children);

    /**
     * @brief Remove the child.
     *
     * @param child The GameObject being removed.
     */
    void RemoveChild(std::shared_ptr<ShapezObject> child);

    /**
     * @brief Draw children according to their z-index.
     *
     * @note The user is not recommended to modify this function.
     */
    void Update();

private:
    std::vector<std::shared_ptr<ShapezObject>> m_Children;
    std::vector<std::shared_ptr<ShapezObject>> stack;
    std::vector<std::shared_ptr<ShapezObject>> buckets[101]; // 2d array for sorting
};

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_SHAPEZRENDERER_HPP