#pragma once

#include "FieldTypeUtils.h"

#include <glm/vec2.hpp>
#include <map>

class Field;

class WafeFunctionCollapseGenerator
{
    public:
        explicit WafeFunctionCollapseGenerator(const glm::ivec2& dimensions, const long& seed = 0, const bool debugOutput = false);
        ~WafeFunctionCollapseGenerator() = default;

        void generateGrid();
        bool generateNextField();
        void initializeGrid();
        bool presetField(const glm::ivec2& pos, const BasicFieldDataStruct& tileType);
        const glm::ivec2 getFieldForFieldType(const BasicFieldDataStruct& tileType) const;

        void addFieldTypes(const std::vector<BasicFieldDataStruct>& fieldTypes);

        static inline glm::ivec2 GRID_SIZE = glm::ivec2(0);

    protected:
        virtual void setField(const std::shared_ptr<Field>& pos, const BasicFieldDataStruct& tileType);
        virtual void setFieldCallback(const std::shared_ptr<Field>& pos, const BasicFieldDataStruct& tileType) = 0;

        long m_seed;
        std::vector<std::vector<std::shared_ptr<Field>>> m_grid;
        std::vector<BasicFieldDataStruct> m_allFieldTypes;
        bool m_generated;
        bool m_initialized;

        bool m_debugMode;
        std::vector<double> m_timeSpentPickingFields;
        std::vector<double> m_timeSpentSettingFields;

    private:
        const std::shared_ptr<Field> pickNextField() const;
};
