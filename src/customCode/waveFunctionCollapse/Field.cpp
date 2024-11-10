
#include "Field.h"

Field::Field(const std::vector<BasicFieldDataStruct>& fieldTypes)
    : m_fieldSet(false)
    , m_fieldPos(glm::ivec2())
    , m_possibleFieldTypes(fieldTypes)
{
}

void Field::updateNeighboringFields(const std::vector<std::vector<std::shared_ptr<Field>>>& field)
{
    for(glm::ivec2 offset : GetNeighborOffsets())
    {
        offset += m_fieldPos;

        if(offset.x < 0 || offset.x >= GRID_SIZE.x || offset.y < 0 || offset.y >= GRID_SIZE.y)
        {
            continue; // Out of bounds
        }

        field[offset.x][offset.y]->updatePossibleFields(field);
    }
}

void Field::updatePossibleFields(const std::vector<std::vector<std::shared_ptr<Field>>>& field)
{
    if(m_possibleFieldTypes.size() == 1)
    {
        return;
    }

    std::vector<BasicFieldDataStruct> possibleFields = m_possibleFieldTypes;
    for(glm::ivec2 offset : GetNeighborOffsets())
    {
        offset += m_fieldPos;

        if(offset.x < 0 || offset.x >= GRID_SIZE.x || offset.y < 0 || offset.y >= GRID_SIZE.y)
        {
            continue; // Out of bounds
        }

        const std::vector<BasicFieldDataStruct>& possibleFieldTypesOfNeighborField =
                field[offset.x][offset.y]->getAllPossibleFieldTypes();

        std::vector<BasicFieldDataStruct> fieldsToRemove;
        for(const BasicFieldDataStruct& possibleFieldType : possibleFields)
        {
            bool found = false;
            for(const BasicFieldDataStruct& possibleNeighborFieldType : possibleFieldTypesOfNeighborField)
            {
                auto it = std::find(
                        possibleNeighborFieldType.allowedNeighborTileTypeIds.begin(),
                        possibleNeighborFieldType.allowedNeighborTileTypeIds.end(),
                        possibleFieldType.tileTypeId
                );
                if(it != possibleNeighborFieldType.allowedNeighborTileTypeIds.end())
                {
                    found = true;
                    break;
                }
            }

            if(!found)
            {
                fieldsToRemove.push_back(possibleFieldType);
            }

            if(possibleFields.size() - fieldsToRemove.size() == 1)
            {
                break;
            }
        }

        possibleFields.erase(
                std::remove_if(
                        possibleFields.begin(),
                        possibleFields.end(),
                        [&](const BasicFieldDataStruct& fieldToCheck) {
                            return std::find(fieldsToRemove.begin(), fieldsToRemove.end(), fieldToCheck) !=
                                    fieldsToRemove.end();
                        }
                ),
                possibleFields.end()
        );

        if(possibleFields.size() == 1)
        {
            break;
        }
    }

    if(m_possibleFieldTypes.size() != possibleFields.size())
    {
        m_possibleFieldTypes = possibleFields;
        updateNeighboringFields(field);
    }
}

void Field::setField(BasicFieldDataStruct type, const std::vector<std::vector<std::shared_ptr<Field>>>& field)
{
    m_possibleFieldTypes.clear();
    m_possibleFieldTypes.push_back(type);
    m_fieldSet = true;

    updateNeighboringFields(field);

    /*
    static const float startPosX = (FIELD_SIZE.x * ((float)GRID_SIZE.x - 1.f)) / 2.f;
    static const float startPosY = (FIELD_SIZE.y * ((float)GRID_SIZE.y - 1.f)) / 2.f;

    const float posX = (m_fieldPos.x * FIELD_SIZE.x) - startPosX;
    const float posY = (m_fieldPos.y * FIELD_SIZE.y) - startPosY;

    std::shared_ptr<Engine::GeometryComponent> planeObj = std::make_shared<Engine::GeometryComponent>();
    planeObj->setObjectData(renderManager->registerObject("resources/objects/plane.obj"));
    planeObj->setShader(std::make_shared<ColorShader>(renderManager));
    planeObj->setRotation(glm::vec3(-90.f, 0.f, 0.f));
    planeObj->setPosition(glm::vec3(posX, 0.f, posY));

    std::vector<glm::vec4> g_color_buffer_data;
    for(int v = 0; v < planeObj->getObjectData()->getVertexCount(); v++)
    {
        g_color_buffer_data.emplace_back(EnumToColorValue(type), 1.f);
    }

    planeObj->setTextureBuffer(renderManager->createBuffer(g_color_buffer_data));

    return planeObj;
     */
}
