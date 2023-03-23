#pragma once

#include <common\types\forward\Vector2Dfwd.hpp>
#include <common\types\forward\Vector3Dfwd.hpp>
#include <common\types\forward\Vector4Dfwd.hpp>

template<class Ty>
inline udon::Vector2D<Ty> udon::Vector3D<Ty>::xy() const
{
    return { x, y };
}

template<class Ty>
inline udon::Vector4D<Ty> udon::Vector3D<Ty>::xyz0() const
{
    return { x, y, z, 0 };
}
