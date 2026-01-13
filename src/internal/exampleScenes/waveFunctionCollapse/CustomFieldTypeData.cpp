
#include "CustomFieldTypeData.h"

template<typename T>
static inline bool DontSqueezeBetweenT(const glm::ivec2& pos, const grid2d& grid)
{
    static_assert(
            std::is_same<BasicFieldDataStruct, decltype(static_cast<BasicFieldDataStruct>(T()))>::value,
            "BasicFieldDataStruct::DontSqueezeBetweenT() requires a type of decendand BasicFieldDataStruct"
    );

    const BasicFieldDataStruct fieldType = static_cast<BasicFieldDataStruct>(T());
    for(const std::vector<glm::ivec2>& offsetGroup : GetGroupedCornerNeighborOffsets())
    {
        const glm::ivec2 cornerPiece = offsetGroup[1] + pos;

        if(cornerPiece.x < 0 || cornerPiece.x >= grid.size() || cornerPiece.y < 0 ||
           cornerPiece.y >= grid.front().size())
        {
            continue; // At least one out of bounds
        }

        if(grid[cornerPiece.x][cornerPiece.y]->getIsSetAsFieldType(fieldType))
        {
            continue;
        }

        const glm::ivec2 directNeighbor1 = offsetGroup[0] + pos;
        const glm::ivec2 directNeighbor2 = offsetGroup[2] + pos;

        if(grid[directNeighbor1.x][directNeighbor1.y]->getIsSetAsFieldType(fieldType) &&
           grid[directNeighbor2.x][directNeighbor2.y]->getIsSetAsFieldType(fieldType))
        {
            return false;
        }
    }

    return true;
}

template<typename T>
static inline bool PreventSingleCornersOfT(const glm::ivec2& pos, const grid2d& grid)
{
    static_assert(
            std::is_same<BasicFieldDataStruct, decltype(static_cast<BasicFieldDataStruct>(T()))>::value,
            "BasicFieldDataStruct::PreventSingleCornersOfT() requires a type of decendand BasicFieldDataStruct"
    );

    const BasicFieldDataStruct fieldType = static_cast<BasicFieldDataStruct>(T());
    for(const std::vector<glm::ivec2>& offsetGroup : GetGroupedCornerNeighborOffsets())
    {
        const glm::ivec2 cornerPiece = offsetGroup[1] + pos;

        if(cornerPiece.x < 0 || cornerPiece.x >= grid.size() || cornerPiece.y < 0 ||
           cornerPiece.y >= grid.front().size())
        {
            continue; // Out of bounds
        }

        if(!grid[cornerPiece.x][cornerPiece.y]->getIsSetAsFieldType(fieldType))
        {
            continue;
        }

        const bool neighbor1CanT =
                grid[(offsetGroup[0] + pos).x][(offsetGroup[0] + pos).y]->getCanBeFieldType(fieldType);
        const bool neighbor2CanT =
                grid[(offsetGroup[2] + pos).x][(offsetGroup[2] + pos).y]->getCanBeFieldType(fieldType);

        if(!neighbor1CanT && !neighbor2CanT)
        {
            return false;
        }
    }

    return true;
}

template<typename T1, typename T2>
static inline bool CheckAllNeighborsCanBeOfTypesT(const glm::ivec2& pos, const grid2d& grid)
{
    return CheckAllNeighborsCanBeOfTypesT<T1, T2, T2>(pos, grid);
}

template<typename T1, typename T2, typename T3>
static inline bool CheckAllNeighborsCanBeOfTypesT(const glm::ivec2& pos, const grid2d& grid)
{
    static_assert(
            std::is_same<BasicFieldDataStruct, decltype(static_cast<BasicFieldDataStruct>(T1()))>::value,
            "BasicFieldDataStruct::CheckAllNeighborsCanBeOfTypesT() requires a type of decendand BasicFieldDataStruct"
    );
    static_assert(
            std::is_same<BasicFieldDataStruct, decltype(static_cast<BasicFieldDataStruct>(T2()))>::value,
            "BasicFieldDataStruct::CheckAllNeighborsCanBeOfTypesT() requires a type of decendand BasicFieldDataStruct"
    );
    static_assert(
            std::is_same<BasicFieldDataStruct, decltype(static_cast<BasicFieldDataStruct>(T3()))>::value,
            "BasicFieldDataStruct::CheckAllNeighborsCanBeOfTypesT() requires a type of decendand BasicFieldDataStruct"
    );

    const BasicFieldDataStruct fieldType1 = static_cast<BasicFieldDataStruct>(T1());
    const BasicFieldDataStruct fieldType2 = static_cast<BasicFieldDataStruct>(T2());
    const BasicFieldDataStruct fieldType3 = static_cast<BasicFieldDataStruct>(T3());
    for(glm::ivec2 offset : GetNeighborOffsets())
    {
        offset += pos;
        if(offset.x < 0 || offset.x >= grid.size() || offset.y < 0 || offset.y >= grid.front().size())
        {
            continue; // Out of bounds
        }

        const std::shared_ptr<Field> field = grid[offset.x][offset.y];
        const bool canBeT1 = field->getCanBeFieldType(fieldType1);
        const bool canBeT2 = field->getCanBeFieldType(fieldType2);
        const bool canBeT3 = field->getCanBeFieldType(fieldType3);

        if(!canBeT1 && !canBeT2 && !canBeT3)
        {
            return false;
        }
    }

    return true;
}

DeepWaterFieldDataStruct::DeepWaterFieldDataStruct()
    : BasicFieldDataStruct(
              FieldTypeEnum::deepWater,
              1,
              { CheckAllNeighborsCanBeOfTypesT<ShallowWaterFieldDataStruct, DeepWaterFieldDataStruct>,
                DontSqueezeBetweenT<ShallowWaterFieldDataStruct> }
      )
{
}

ShallowWaterFieldDataStruct::ShallowWaterFieldDataStruct()
    : BasicFieldDataStruct(
              FieldTypeEnum::shallowWater,
              2,
              { CheckAllNeighborsCanBeOfTypesT<ShallowWaterFieldDataStruct, DeepWaterFieldDataStruct, BeachFieldDataStruct>,
                DontSqueezeBetweenT<BeachFieldDataStruct>,
                DontSqueezeBetweenT<DeepWaterFieldDataStruct> }
      )
{
}

BeachFieldDataStruct::BeachFieldDataStruct()
    : BasicFieldDataStruct(
              FieldTypeEnum::beach,
              2,
              { CheckAllNeighborsCanBeOfTypesT<ShallowWaterFieldDataStruct, BeachFieldDataStruct, GrasFieldDataStruct>,
                PreventSingleCornersOfT<BeachFieldDataStruct>,
                DontSqueezeBetweenT<GrasFieldDataStruct> }
      )
{
}

GrasFieldDataStruct::GrasFieldDataStruct()
    : BasicFieldDataStruct(
              FieldTypeEnum::grass,
              10,
              { CheckAllNeighborsCanBeOfTypesT<StoneFieldDataStruct, BeachFieldDataStruct, GrasFieldDataStruct>,
                DontSqueezeBetweenT<StoneFieldDataStruct>,
                DontSqueezeBetweenT<BeachFieldDataStruct> }
      )
{
}

StoneFieldDataStruct::StoneFieldDataStruct()
    : BasicFieldDataStruct(
              FieldTypeEnum::stone,
              1,
              {
                      CheckAllNeighborsCanBeOfTypesT<HillFieldDataStruct, GrasFieldDataStruct, StoneFieldDataStruct>,
                      PreventSingleCornersOfT<StoneFieldDataStruct>,
                      DontSqueezeBetweenT<HillFieldDataStruct>,
                      DontSqueezeBetweenT<GrasFieldDataStruct>,
              }
      )
{
}

HillFieldDataStruct::HillFieldDataStruct()
    : BasicFieldDataStruct(
              FieldTypeEnum::hill,
              2,
              {
                      CheckAllNeighborsCanBeOfTypesT<HillFieldDataStruct, StoneFieldDataStruct, MountainFieldDataStruct>,
                      PreventSingleCornersOfT<HillFieldDataStruct>,
                      DontSqueezeBetweenT<MountainFieldDataStruct>,
              }
      )
{
}

MountainFieldDataStruct::MountainFieldDataStruct()
    : BasicFieldDataStruct(
              FieldTypeEnum::mountain,
              1,
              {
                      CheckAllNeighborsCanBeOfTypesT<HillFieldDataStruct, MountainFieldDataStruct>,
                      PreventSingleCornersOfT<MountainFieldDataStruct>,
              }
      )
{
}
