#define _USE_MATH_DEFINES

#include <iostream>

#include "../../includes//World//Camera.h"
#include "../../includes/Rendering/Rasterizer.h"

using namespace World;
using namespace Rendering;

float Camera::m_near = 0.f;
float Camera::m_far = 0.f;

Camera::Camera(const int& p_width,
               const int& p_height,
               const float& p_near,
               const float& p_far,
               const float& p_fov,
               const Vec3& p_position) : m_top {static_cast<float>(tan(p_fov * 0.5f * (M_PI / 180.f)) * p_near)},
                                         m_right {m_top * static_cast<float>(p_width / static_cast<float>(p_height))},
                                         m_viewMatrix {CreateViewMatrix(p_position)},
                                         m_projectionMatrix {Rasterizer::CreatePerspectiveProjectionMatrix(p_near, p_far, m_top, m_right)}
{
	m_near = p_near;
	m_far = p_far;

//	std::cout << "Camera created" << std::endl;
}

Camera::~Camera()
{
//	std::cout << "Camera destroyed" << std::endl;
}

void Camera::SetViewMatrix(const Vec3& p_position) { m_viewMatrix = CreateViewMatrix(p_position); }
void Camera::SetProjectionMatrix(const Mat4& p_projectionMatrix) { m_projectionMatrix = p_projectionMatrix; }

Mat4 Camera::CreateViewMatrix(const Vec3& p_position)
{
	return Mat4::CreateTranslationMatrix(p_position).GetInverse();
}

const Mat4& Camera::GetViewMatrix() const { return m_viewMatrix; }
const Mat4& Camera::GetProjectionMatrix() const { return m_projectionMatrix; }
