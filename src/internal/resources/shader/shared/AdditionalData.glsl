// Arguments: [0] = data type (e.g. mat4, vec3, etc)

#include <"resources/shader/shared/BasicData.glsl">

layout(location = 7) in arg[0] additionalDataInstanced;

uniform arg[0] additionalDataUni;

arg[0] getAdditionalData()
{
    if(isInstanced)
    {
        return additionalDataInstanced;
    }
    else
    {
        return additionalDataUni;
    }
}