#pragma once

#include "FieldTypeUtils.h"

#include <glm/ext/vector_int2.hpp>
#include <glm/vec2.hpp>

class Field
{
    public:
        explicit Field(const std::vector<BasicFieldDataStruct>& fieldTypes);
        ~Field() = default;

        void updatePossibleFields(const std::vector<std::vector<std::shared_ptr<Field>>>& field);

        void setField(BasicFieldDataStruct type, const std::vector<std::vector<std::shared_ptr<Field>>>& field);

        void updateNeighboringFields(const std::vector<std::vector<std::shared_ptr<Field>>>& field);

        void setPosition(const glm::ivec2& pos) { m_fieldPos = pos; }

        bool getIsFieldSet() const { return m_fieldSet; }

        std::vector<BasicFieldDataStruct> getAllPossibleFieldTypes() const { return m_possibleFieldTypes; }

        static void setGridSize(const glm::ivec2& gridSize) { GRID_SIZE = gridSize; }

        static void setFieldSize(const glm::ivec2& fieldSize) { FIELD_SIZE = fieldSize; }

    private:
        static inline glm::ivec2 GRID_SIZE = glm::ivec2(0);
        static inline glm::vec2 FIELD_SIZE = glm::vec2(0.f);

        glm::ivec2 m_fieldPos;
        bool m_fieldSet;
        std::vector<BasicFieldDataStruct> m_possibleFieldTypes;
};
