#include "camera.h"
#include "utils/util.h"
#include "system/system.h"
#include "window/window.h"

#include <SDL3/SDL.h>

ENGINE_NAMESPACE_BEGIN

Camera::~Camera()
{
	
}

void Camera::initialize()
{
    // initialize look up
    update_clip_space(m_position, m_center, m_world_up);
}

void Camera::update(GlobalRuntimeInfo& global_info)
{
    // clip space
    Vec3f offset{ 0.0, 0.0, 0.0 };

    Float delta_time = global_info.current_time - global_info.previous_time;

    auto& events = global_info.events;
    for (auto it = events.begin(); it != events.end();)
    {
        switch (it->type)
        {
        case SDL_EVENT_KEY_DOWN:
        {
            if (it->key.down)
            {
                switch (it->key.scancode)
                {
                case SDL_SCANCODE_A:
                    offset.x() -= m_move_speed * delta_time;
                    break;
                case SDL_SCANCODE_D:
                    offset.x() += m_move_speed * delta_time;
                    break;
                case SDL_SCANCODE_W:
                    offset.z() -= m_move_speed * delta_time;
                    break;
                case SDL_SCANCODE_S:
                    offset.z() += m_move_speed * delta_time;
                    break;
                case SDL_SCANCODE_ESCAPE:
                    global_info.window_quit = true;
                    break;
                case SDL_SCANCODE_SPACE:
                    global_info.render_stop = !global_info.render_stop;
                    break;
                default:
                    break;
                }
            }

            it = events.erase(it);
        }
            break;
        case SDL_EVENT_MOUSE_MOTION:
        {
            int width, height;
            g_system_context->g_window_system->window_size(&width, &height);

            Float yaw = static_cast<Float>(it->motion.xrel) / static_cast<Float>(width) * m_mouse_sensitivity;
            Float pitch = static_cast<Float>(it->motion.yrel) / static_cast<Float>(height) * m_mouse_sensitivity;

            Mat3f r_inv = (Eigen::AngleAxisf(yaw, m_up) * Eigen::AngleAxisf(pitch, m_right)).matrix().transpose();

            Mat3f rotation;
            rotation.col(2) = m_front;
            rotation.col(0) = m_right;
            rotation.col(1) = m_up;

            Mat3f compose = r_inv * rotation;
            m_front = compose.col(2);
            m_right = compose.col(0);
            m_up = compose.col(1);

            it = events.erase(it);
        }
            break;
        case SDL_EVENT_KEY_UP:
            it = events.erase(it);
            break;
        default:
            it++;
            break;
        }
    }

    Mat3f rotation;
    rotation.col(2) = m_front;
    rotation.col(0) = m_right;
    rotation.col(1) = m_up;

    m_position += rotation * offset;
}

Affine3f Camera::perspective(Float aspect) const
{
    float theta = m_fov * 0.5;
    float range = m_z_far - m_z_near;
    float inv_tan = 1.0 / std::tan(theta);

    Affine3f ret(Affine3f::Identity());
    ret(0, 0) = inv_tan / aspect;
    ret(1, 1) = inv_tan;
    ret(2, 2) = -(m_z_far + m_z_near) / range;
    ret(2, 3) = -2.0 * m_z_near * m_z_far / range;
    ret(3, 2) = -1.0;
    ret(3, 3) = 0.0;

    return ret;
}

Affine3f Camera::look_at() const
{
    Mat3f rotation;
    rotation.col(2) = m_front;
    rotation.col(0) = m_right;
    rotation.col(1) = m_up;

    Affine3f ret(Affine3f::Identity());
    ret.linear() = rotation.transpose();
    ret.translation() = -ret.linear() * m_position;

    return ret;
}

void Camera::update_clip_space(const Vec3f& eye, const Vec3f& target, const Vec3f& world)
{
    m_front = (eye - target).normalized();
    m_right = world.cross(m_front).normalized();
    m_up = m_front.cross(m_right);
}


ENGINE_NAMESPACE_END