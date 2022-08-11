#pragma once
#include  "core/GLM.h"
#include "BV.h"

struct Cube
{
    Vector3 Position;
    Vector3 Width;
    operator aabb() const
    {
        return aabb{ Position + Width, Position - Width };
    }
};