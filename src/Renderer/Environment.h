#include "core/core.h"
#include "scene/Light.h"

struct Environment
{
    Light* DirLight = nullptr;
    bool HasShadow = false;
};