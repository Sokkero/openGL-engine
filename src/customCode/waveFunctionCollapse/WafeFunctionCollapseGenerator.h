#pragma once

#include "FieldTypeUtils.h"

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

    private:
        virtual void setField(const glm::ivec2& pos, const BasicFieldDataStruct& tileType) = 0;

        const glm::ivec2 pickNextField() const;

        long m_seed;
        std::vector<std::vector<std::shared_ptr<Field>>> m_grid;
        const glm::ivec2 m_gridDimensions;
        std::vector<BasicFieldDataStruct> m_fieldTypes;
        bool m_generated;
        bool m_initialized;
};
