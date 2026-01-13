#pragma once

#include "classes/nodeComponents/BasicNode.h"
#include "classes/uiElements/UiElement.h"

#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace Engine::Ui
{
    /**
     * @brief Represents a debug window for user interface elements.
     *
     * This class provides functionality to create and manage a debug window that can display various UI
     * elements. It inherits from UiElement and BasicNode classes.
     */
    class UiDebugWindow
        : protected UiElement
        , virtual public BasicNode
    {
        public:
            /**
             * @brief Constructs a UiDebugWindow object.
             *
             * @param flags The ImGui window flags for the debug window.
             */
            explicit UiDebugWindow(ImGuiWindowFlags flags = ImGuiWindowFlags_None);
            ~UiDebugWindow() = default;

            /**
             * @brief Draws the UI elements inside the debug window.
             *
             * This function is called to render the UI elements inside the debug window.
             * It overrides the drawUi() function from the UiElement class.
             */
            void drawUi() override;

            /**
             * @brief Gets the content of the debug window.
             *
             * @return A vector of shared pointers to UiElement primitives representing the content of the debug window.
             */
            std::vector<std::shared_ptr<UiElement>> getContent() const { return m_content; };

            /**
             * @brief Adds a new UI element to the debug window.
             *
             * @param newContent A shared pointer to a UiElement object representing the new UI element to be added.
             */
            void addContent(const std::shared_ptr<UiElement>& newContent)
            {
                m_content.emplace_back(newContent);
            };

            /**
             * @brief Removes a UI element from the debug window.
             *
             * @param content A shared pointer to a UiElement object representing the UI element to be removed.
             */
            void removeContent(const std::shared_ptr<UiElement>& content)
            {
                m_content.erase(std::remove(m_content.begin(), m_content.end(), content), m_content.end());
            }

            /**
             * @brief Checks if the debug window is open.
             *
             * @return True if the debug window is open, false otherwise.
             */
            bool isWindowOpen() const { return m_windowOpen; };

            /**
             * @brief Sets the state of the debug window.
             *
             * @param isWindowOpen True to open the debug window, false to close it.
             */
            void setWindowOpen(bool isWindowOpen) { m_windowOpen = isWindowOpen; };

            /**
             * @brief Gets the ImGui window flags of the debug window.
             *
             * @return The ImGui window flags of the debug window.
             */
            ImGuiWindowFlags getWindowFlags() const { return m_flags; };

            /**
             * @brief Adds an ImGui window flag to the debug window.
             *
             * @param flag The ImGui window flag to be added.
             */
            void addWindowFlag(ImGuiWindowFlags flag) { m_flags = m_flags | flag; };

            /**
             * @brief Removes an ImGui window flag from the debug window.
             *
             * @param flag The ImGui window flag to be removed.
             */
            void removeWindowFlag(ImGuiWindowFlags flag) { m_flags = m_flags & (~flag); };

            /**
             * @brief Gets the title of the debug window.
             *
             * @return The title of the debug window.
             */
            std::string getWindowTitle() const { return m_windowTitle; };

            /**
             * @brief Sets the title of the debug window.
             *
             * @param title The new title of the debug window.
             */
            void setWindowTitle(std::string title)
            {
                m_windowTitle = std::move(title);
                setName(m_windowTitle);
            };

            bool getIsWindowClosable() const { return m_windowIsClosable; };

            void setIsWindowClosable(bool isClosable) { m_windowIsClosable = std::move(isClosable); };

        private:
            std::vector<std::shared_ptr<UiElement>> m_content;
            bool m_windowOpen;
            bool m_windowIsClosable;
            ImGuiWindowFlags m_flags;
            std::string m_windowTitle = "Hello, World!";
    };
} // namespace Engine::Ui
