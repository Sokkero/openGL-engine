#pragma once

#include "FieldTypeUtils.h"

#include <glm/vec2.hpp>
#include <map>

class Field;

class WafeFunctionCollapseGenerator
{
    public:
        explicit WafeFunctionCollapseGenerator(const glm::ivec2& dimensions, const long& seed = 0);
        ~WafeFunctionCollapseGenerator() = default;

        void generateGrid();
        void initializeGrid();
        bool presetField(const glm::ivec2& pos, const BasicFieldDataStruct& tileType);
        const glm::ivec2 getFieldForFieldType(const BasicFieldDataStruct& tileType) const;

        void addFieldTypes(const std::vector<BasicFieldDataStruct>& fieldTypes);

        static inline glm::ivec2 GRID_SIZE = glm::ivec2(0);

    protected:
        virtual void setField(const glm::ivec2& pos, const BasicFieldDataStruct& tileType);
        virtual void setFieldCallback(const glm::ivec2& pos, const BasicFieldDataStruct& tileType) = 0;

        long m_seed;
        std::vector<std::vector<std::shared_ptr<Field>>> m_grid;
        std::vector<BasicFieldDataStruct> m_allFieldTypes;
        bool m_generated;
        bool m_initialized;

    private:
        const glm::ivec2 pickNextField() const;
};
