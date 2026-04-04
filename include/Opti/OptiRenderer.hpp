//
// Created by joshb on 2026/4/1.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_SHAPEZRENDERER_HPP
#define REPLACE_WITH_YOUR_PROJECT_NAME_SHAPEZRENDERER_HPP

#include <vector>
#include <memory>
#include "Util/GameObject.hpp"
#include "OptiObject.hpp"
#include "Util/Image.hpp"
#include "Core/Drawable.hpp"
#include "OptiImage.hpp"

class App;

/**
 * @class OptiRenderer
 * @brief A class optimized for calling shapez object's Draw()
 * @see Util::GameObject
 */

struct DrawCommand {
    OptiImage* image;
    Core::Matrices data;
};

class OptiRenderer final {
public:
    /**
     * @brief Parameterized constructor.
     *`
     *
     * @param children The GameObject needing to be managed by Renderer.
     */
    OptiRenderer(const std::vector<std::shared_ptr<OptiObject>> &children = {});

    /**
     * @brief Add a child to Renderer.
     *
     * @param child The GameObject needing to be managed by Renderer.
     */
    void AddChild(const std::shared_ptr<OptiObject> &child);

    /**
     * @brief Add children to Renderer.
     *
     * @param children The GameObjects needing to be managed by Renderer.
     */
    void AddChildren(const std::vector<std::shared_ptr<OptiObject>> &children);

    /**
     * @brief Remove the child.
     *
     * @param child The GameObject being removed.
     */
    void RemoveChild(std::shared_ptr<OptiObject> child);

    /**
     * @brief Draw children according to their z-index.
     *
     * @note The user is not recommended to modify this function.
     */
    void Update();

    static void Submit(OptiImage* img, const Core::Matrices& data);

private:
    static inline std::vector<DrawCommand> commands;
    std::vector<std::shared_ptr<OptiObject>> m_Children;
    std::vector<OptiObject*> current;
    std::vector<OptiObject*> buckets[101]; // 2d array for sorting

    /*
    static inline std::unique_ptr<Core::Program> s_Program = std::make_unique<Core::Program>("../PTSD/assets/shaders/Base.vert",
                                        "../PTSD/assets/shaders/Base.frag");
    static inline std::unique_ptr<Core::VertexArray> s_VertexArray = std::make_unique<Core::VertexArray>();
    */
};

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_SHAPEZRENDERER_HPP