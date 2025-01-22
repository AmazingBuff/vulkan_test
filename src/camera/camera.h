#pragma once

#include "base/macro.h"
#include "system/structure.h"
#include "base/geometry.h"

ENGINE_NAMESPACE_BEGIN

/* 
	model space (right hand coordinate)
	(front, right, up	)
	(x,		y,	   z	)
  
	clip space (right hand coordinate)
	(right, up,	   front)
	(x,		y,	   z	)
*/
class Camera final
{
public:
	Camera() = default;
	~Camera();
	void initialize();

	void update(GlobalRuntimeInfo& global_info);

	Affine3f perspective(Float aspect) const;
	Affine3f look_at() const;
private:
	void update_clip_space(const Vec3f& eye, const Vec3f& target, const Vec3f& world);
private:
	// perspective
	Float m_fov = 45.0;
	Float m_z_near = 0.1;
	Float m_z_far = 10.0;

	// view
	Vec3f m_position{3.0, 0.0, 0.0};
	Vec3f m_center{ 0.0, 0.0, 0.0 };
	Vec3f m_world_up{ 0.0, 0.0, 1.0 };

	Vec3f m_right{ 1.0, 0.0, 0.0 };
	Vec3f m_front{ 0.0, 0.0, 1.0 };
	Vec3f m_up{ 0.0, 1.0, 0.0 };

	// speed
	Float m_move_speed = 10.0;

	// mouse
	Float m_mouse_x = 0.0;
	Float m_mouse_y = 0.0;
	Float m_mouse_sensitivity = 1.0;
};


ENGINE_NAMESPACE_END