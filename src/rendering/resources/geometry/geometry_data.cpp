#include "geometry_data.h"

ENGINE_NAMESPACE_BEGIN

Affine3f look_at(const Vec3f& eye, const Vec3f& target, const Vec3f& up)
{
    Mat3f rotation;
    rotation.col(2) = (eye - target).normalized();
    rotation.col(0) = up.cross(rotation.col(2)).normalized();
    rotation.col(1) = rotation.col(2).cross(rotation.col(0));

    Affine3f ret(Affine3f::Identity());
    ret.linear() = rotation.transpose();
    ret.translation() = -rotation.transpose() * eye;

    return ret;
}

Affine3f perspective(Float fov, Float aspect, Float z_near, Float z_far)
{
    float theta = fov * 0.5;
    float range = z_far - z_near;
    float inv_tan = 1.0 / std::tan(theta);

    Affine3f ret(Affine3f::Identity());
    ret(0, 0) = inv_tan / aspect;
    ret(1, 1) = inv_tan;
    ret(2, 2) = -(z_far + z_near) / range;
    ret(2, 3) = -2.0 * z_near * z_far / range;
    ret(3, 2) = -1.0;
    ret(3, 3) = 0.0;

    return ret;
}


ENGINE_NAMESPACE_END