//
// Created by AmazingBuff on 2024/12/20.
//

#pragma once

#include "macro.h"
#include "util.h"
#include <Eigen/core>
#include <Eigen/Geometry>

AMAZING_NAMESPACE_BEGIN

using Vec2f = Eigen::Vector2<Float>;
using Vec3f = Eigen::Vector3<Float>;
using Vec4f = Eigen::Vector4<Float>;

using Vec2i = Eigen::Vector2<int32_t>;
using Vec2u = Eigen::Vector2<uint32_t>;

using Mat3f = Eigen::Matrix<Float, 3, 3>;
using Mat4f = Eigen::Matrix<Float, 4, 4>;

using Affine3f = Eigen::Affine3f;

AMAZING_NAMESPACE_END

template <typename Scalar, int Rows, int Cols>
struct std::hash<Eigen::Matrix<Scalar, Rows, Cols>> 
{
    // https://wjngkoh.wordpress.com/2015/03/04/c-hash-function-for-eigen-matrix-and-vector/
    NODISCARD constexpr size_t operator()(const Eigen::Matrix<Scalar, Rows, Cols>& matrix) const 
    {
        size_t seed = 0;
        for (size_t i = 0; i < matrix.size(); i++) 
        {
            Scalar elem = *(matrix.data() + i);
            seed = Amazing::hash_combine(seed, hash<Scalar>()(elem));
        }
        return seed;
    }
};
