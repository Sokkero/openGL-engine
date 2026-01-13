#pragma once

#include "FieldTypeUtils.h"

#include <glm/ext/vector_int2.hpp>
#include <glm/vec2.hpp>

class Field
{
    public:
        explicit Field(const std::vector<BasicFieldDataStruct>& fieldTypes);
        ~Field() = default;

        void updatePossibleFields(const std::vector<std::vector<std::shared_ptr<Field>>>& grid);

        void setField(const BasicFieldDataStruct& type, const std::vector<std::vector<std::shared_ptr<Field>>>& grid);

        void updateNeighboringFields(const std::vector<std::vector<std::shared_ptr<Field>>>& grid);

        void setPosition(const glm::ivec2& pos) { m_fieldPos = pos; }

        const glm::ivec2 getPosition() const { return m_fieldPos; }

        bool getIsFieldSet() const { return m_fieldSet; }

        std::vector<BasicFieldDataStruct> getAllPossibleFieldTypes() const { return m_possibleFieldTypes; }

        bool getCanBeFieldType(const BasicFieldDataStruct& fieldType) const;
        bool getIsSetAsFieldType(const BasicFieldDataStruct& fieldType) const;

    private:
        glm::ivec2 m_fieldPos;
        bool m_fieldSet;
        std::vector<BasicFieldDataStruct> m_possibleFieldTypes;
};
