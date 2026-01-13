#pragma once

#include "exampleScenes/waveFunctionCollapse/Field.h"
#include "exampleScenes/waveFunctionCollapse/FieldTypeUtils.h"

enum FieldTypeEnum
{
    deepWater = 1,
    shallowWater = 2,
    beach = 3,
    grass = 4,
    stone = 5,
    hill = 6,
    mountain = 7
};

inline static glm::vec3 EnumToColorValue(int field)
{
    switch(field)
    {
        case FieldTypeEnum::deepWater:
            return glm::vec3(0.0f, 0.0f, 0.545f);
        case FieldTypeEnum::shallowWater:
            return glm::vec3(0.678f, 0.847f, 0.902f);
        case FieldTypeEnum::beach:
            return glm::vec3(1.0f, 1.0f, 0.0f);
        case FieldTypeEnum::grass:
            return glm::vec3(0.0f, 0.502f, 0.0f);
        case FieldTypeEnum::stone:
            return glm::vec3(0.6f, 0.6f, 0.6f);
        case FieldTypeEnum::hill:
            return glm::vec3(0.5f, 0.2f, 0.f);
        case FieldTypeEnum::mountain:
            return glm::vec3(0.6f, 0.f, 0.1f);
        default:
            assert(false);
    }
}

struct DeepWaterFieldDataStruct : BasicFieldDataStruct
{
        DeepWaterFieldDataStruct();
        ~DeepWaterFieldDataStruct() = default;
};

struct ShallowWaterFieldDataStruct : BasicFieldDataStruct
{
        ShallowWaterFieldDataStruct();
        ~ShallowWaterFieldDataStruct() = default;
};

struct BeachFieldDataStruct : BasicFieldDataStruct
{
        BeachFieldDataStruct();
        ~BeachFieldDataStruct() = default;
};

struct GrasFieldDataStruct : BasicFieldDataStruct
{
        GrasFieldDataStruct();
        ~GrasFieldDataStruct() = default;
};

struct StoneFieldDataStruct : BasicFieldDataStruct
{
        StoneFieldDataStruct();
        ~StoneFieldDataStruct() = default;
};

struct HillFieldDataStruct : BasicFieldDataStruct
{
        HillFieldDataStruct();
        ~HillFieldDataStruct() = default;
};

struct MountainFieldDataStruct : BasicFieldDataStruct
{
        MountainFieldDataStruct();
        ~MountainFieldDataStruct() = default;
};

inline static BasicFieldDataStruct EnumToFieldData(int field)
{
    switch(field)
    {
        case FieldTypeEnum::deepWater:
            return DeepWaterFieldDataStruct();
        case FieldTypeEnum::shallowWater:
            return ShallowWaterFieldDataStruct();
        case FieldTypeEnum::beach:
            return BeachFieldDataStruct();
        case FieldTypeEnum::grass:
            return GrasFieldDataStruct();
        case FieldTypeEnum::stone:
            return StoneFieldDataStruct();
        case FieldTypeEnum::hill:
            return HillFieldDataStruct();
        case FieldTypeEnum::mountain:
            return MountainFieldDataStruct();
        default:
            assert(false);
    }
}
