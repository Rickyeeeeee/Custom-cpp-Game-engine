#pragma once

#include "core/GLM.h"

namespace Math {
    bool DecomposeTransform(const Matrix4& transform, Vector3& outTranslation, Vector3& outRotation, Vector3& outScale);

}