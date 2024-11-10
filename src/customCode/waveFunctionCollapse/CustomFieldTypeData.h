#pragma once

#include "FieldTypeUtils.h"

enum FieldTypeEnum
{
    deepWater = 1,
    shallowWater = 2,
    beach = 3,
    grass = 4,
    hill = 5,
    mountain = 6
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
        case FieldTypeEnum::hill:
            return glm::vec3(0.5f, 0.2f, 0.f);
        case FieldTypeEnum::mountain:
            return glm::vec3(0.6f, 0.f, 0.1f);
        default:
            assert(false);
    }
}

struct HillFieldDataStruct : BasicFieldDataStruct
{
        HillFieldDataStruct()
            : BasicFieldDataStruct(
                      { FieldTypeEnum::grass, FieldTypeEnum::hill, FieldTypeEnum::mountain },
                      EnumToColorValue(FieldTypeEnum::beach),
                      2,
                      FieldTypeEnum::hill
              ) {};
};

struct MountainFieldDataStruct : BasicFieldDataStruct
{
        MountainFieldDataStruct()
            : BasicFieldDataStruct(
                      { FieldTypeEnum::mountain, FieldTypeEnum::hill },
                      EnumToColorValue(FieldTypeEnum::mountain),
                      1,
                      FieldTypeEnum::mountain
              ) {};
};

struct BeachFieldDataStruct : BasicFieldDataStruct
{
        BeachFieldDataStruct()
            : BasicFieldDataStruct(
                      { FieldTypeEnum::beach, FieldTypeEnum::grass, FieldTypeEnum::shallowWater },
                      EnumToColorValue(FieldTypeEnum::beach),
                      2,
                      FieldTypeEnum::beach
              ) {};
};

struct ShallowWaterFieldDataStruct : BasicFieldDataStruct
{
        ShallowWaterFieldDataStruct()
            : BasicFieldDataStruct(
                      { FieldTypeEnum::beach, FieldTypeEnum::deepWater, FieldTypeEnum::shallowWater },
                      EnumToColorValue(FieldTypeEnum::shallowWater),
                      2,
                      FieldTypeEnum::shallowWater
              ) {};
};

struct DeepWaterFieldDataStruct : BasicFieldDataStruct
{
        DeepWaterFieldDataStruct()
            : BasicFieldDataStruct(
                      { FieldTypeEnum::shallowWater, FieldTypeEnum::deepWater },
                      EnumToColorValue(FieldTypeEnum::deepWater),
                      1,
                      FieldTypeEnum::deepWater
              ) {};
};

struct GrasFieldDataStruct : BasicFieldDataStruct
{
        GrasFieldDataStruct()
            : BasicFieldDataStruct(
                      { FieldTypeEnum::beach, FieldTypeEnum::grass, FieldTypeEnum::hill },
                      EnumToColorValue(FieldTypeEnum::grass),
                      20,
                      FieldTypeEnum::grass
              ) {};
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
        case FieldTypeEnum::hill:
            return HillFieldDataStruct();
        case FieldTypeEnum::mountain:
            return MountainFieldDataStruct();
        default:
            assert(false);
    }
}
