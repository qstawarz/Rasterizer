#pragma once

#include "../Maths/Mat4.h"

using namespace Maths;

namespace World
{
	class Camera
	{
	public:
		static float m_near;
		static float m_far;
	private:
		float m_top;
		float m_right;

		Mat4 m_viewMatrix;
		Mat4 m_projectionMatrix;

	public:
		explicit Camera(const int& p_width,
		                const int& p_height,
		                const float& p_near,
		                const float& p_far,
		                const float& p_fov,
		                const Vec3& p_position);
		~Camera();

		void SetViewMatrix(const Vec3& p_position);
		void SetProjectionMatrix(const Mat4& p_projectionMatrix);

		static Mat4 CreateViewMatrix(const Vec3& p_position);

		const Mat4& GetViewMatrix() const;
		const Mat4& GetProjectionMatrix() const;
	};
}
