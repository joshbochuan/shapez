//
// Created by joshb on 2026/4/2.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_SHAPEZOBJECT_HPP
#define REPLACE_WITH_YOUR_PROJECT_NAME_SHAPEZOBJECT_HPP

#include "pch.hpp" // IWYU pragma: export

#include "Core/Drawable.hpp"

#include "Util/Transform.hpp"

/**
 * @class OptiObject
 * @brief A class representing a game object.
 *
 * This class encapsulates the properties and behaviors of a game object.
 * @note This is an abstract class. Inherit from this class to create your own
 * game objects.
 *
 */
class OptiObject {
public:
    Util::Transform m_Transform; // IDC if this should be here.

public:
    /**
     * @brief Default constructor.
     */
    OptiObject() = default;

    /**
     * @brief Parameterized constructor.
     *
     * @param drawable The Util::Drawable component of the game object.
     * @param zIndex The z-index of the game object.
     * @param visible The visibility of the game object.
     * @param children The children of the game object.
     */
    OptiObject(const std::shared_ptr<Core::Drawable> &drawable,
               const float zIndex, const glm::vec2 &pivot = {0, 0},
               const bool visible = true,
               const std::vector<std::shared_ptr<OptiObject>> &children =
                   std::vector<std::shared_ptr<OptiObject>>())
        : m_Drawable(drawable),
          m_Children(children),
          m_ZIndex(zIndex),
          m_Visible(visible),
          m_Pivot(pivot) {}

    /**
     * @brief Copy constructor.
     * @param other
     *
     * @note This is a shallow copy constructor, meaning the m_Drawable points
     * to the same reference as same as `other`'s does.
     */
    OptiObject(const OptiObject &other) = default;

    /**
     * @brief Default move constructor..
     */
    OptiObject(OptiObject &&other) = default;

    /**
     * @brief Default destructor.
     */
    virtual ~OptiObject() = default;

    // Deleted assignment operator.
    OptiObject &operator=(const OptiObject &other) = delete;

    /**
     * @brief Get the z-index of the game object.
     *
     * @return The z-index of the game object.
     */
    float GetZIndex() const { return m_ZIndex; }

    /**
     * @brief Get the transform of the game object.
     *
     * @return The transform of the game object.
     */
    Util::Transform GetTransform() const { return m_Transform; }

    /**
     * @brief Get the size of its drawable component.
     *
     * @return vec2(x, y) representing the size of the drawable component.
     */
    glm::vec2 GetScaledSize() const {
        return m_Drawable->GetSize() * m_Transform.scale;
    };

    /**
     * @brief Get the children of the game object.
     *
     * @return The children of the game object.
     */
    const std::vector<std::shared_ptr<OptiObject>> &GetChildren() const {
        return m_Children;
    }

    /**
     * @brief Set the pivot of the game object.
     *
     * @param pivot The pivot of the game object.
     */
    void SetPivot(const glm::vec2 &pivot) { m_Pivot = pivot; }

    /**
     * @brief Set the z-index of the game object.
     * z-index is used to determine the order in which game objects are drawn,
     * the greater z-index, the topper the its Util::Drawable is
     *
     * @param index The new z-index of the game object.
     */
    void SetZIndex(float index) { m_ZIndex = index; }

    /**
     * @brief Set the drawable component of the game object.
     *
     * @param drawable The new drawable component of the game object.
     */
    void SetDrawable(const std::shared_ptr<Core::Drawable> &drawable) {
        m_Drawable = drawable;
    }

    /**
     * @brief Set the visibility of the game object.
     *
     * @param visible The new visibility of the game object.
     */
    void SetVisible(const bool visible) { m_Visible = visible; }

    /**
     * @brief Add a child to the game object.
     *
     * @param child The new child of the game object.
     */
    void AddChild(const std::shared_ptr<OptiObject> &child) {
        m_Children.push_back(child);
    }

    /**
     * @brief Remove a child from the game object.
     *
     * @param child The child to be removed.
     */
    void RemoveChild(const std::shared_ptr<OptiObject> &child) {
        m_Children.erase(
            std::remove(m_Children.begin(), m_Children.end(), child),
            m_Children.end());
    }

    void CalData(); // called before draw
    void Draw();

    std::shared_ptr<Core::Drawable> m_Drawable = nullptr;
    std::vector<std::shared_ptr<OptiObject>> m_Children;

    Core::Matrices data = {glm::mat4(1.0f), glm::mat4(1.0f)};
    float m_ZIndex = 0;
    bool m_Visible = true;
    glm::vec2 m_Pivot = {0, 0};
};

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_SHAPEZOBJECT_HPP