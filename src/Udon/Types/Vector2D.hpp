//
//    二次元ベクトル
//
//    Copyright (c) 2022-2023 udonrobo
//

#pragma once

#include "Forward/Polar.hpp"
#include "Forward/Vector2D.hpp"
#include "Forward/Vector3D.hpp"

inline Udon::Vector3D Udon::Vector2D::xy0() const noexcept
{
    return { x, y, 0 };
}

inline Udon::Polar Udon::Vector2D::toPolar() const noexcept
{
    return { length(), angle() };
}

namespace Udon
{
    using Vec2 = Vector2D;
}
