#pragma once

#include <vector>

#include "../World/Scene.h"
#include "../Graphics/Texture.h"

using namespace World;
using namespace Graphics;

namespace Rendering
{
	class Rasterizer
	{
	private:
		unsigned int m_width;
		unsigned int m_height;

		float* m_ZBuffer;

	public:
		explicit Rasterizer(const unsigned int& p_width, const unsigned int& p_height);
		~Rasterizer();

		void RenderScene(const Scene* p_scene,
		                 const Texture* p_target,
		                 const SDLManager* p_SDLManager) const;
		void BresenhamCoordinates(const Vertex& p_v0,
		                          const Vertex& p_v1,
		                          const Texture* p_target) const;
		void BresenhamMethod(const Vertex& p_v0,
		                     const Vertex& p_v1,
		                     const Vertex& p_v2,
		                     const Texture* p_target) const;
		void BarycentricMethod(const Vertex& p_v0,
		                       const Vertex& p_v1,
		                       const Vertex& p_v2,
		                       const Texture* p_target) const;
		void Normal(const Vertex& p_v0,
		            const Vertex& p_v1,
		            const Vertex& p_v2,
		            const Mat4& p_projectionMatrix,
		            const Texture* p_target) const;
		void Clipping(std::vector<Vec4>& p_vectors) const;
		void Clear() const;

		Vertex Transform(const Vertex& p_vertex,
		                 const Mat4& p_inverseCameraMatrix,
		                 const Mat4& p_transformMatrix) const;
		Vertex PhongMethod(const Vertex& p_vertex,
		                   const std::vector<Light>& p_lights,
		                   const Mat4& p_inverseCameraMatrix) const;
		Vertex BlinnPhongMethod(const Vertex& p_vertex,
		                        const std::vector<Light>& p_lights,
		                        const Mat4& p_inverseCameraMatrix) const;

		Vec4 Projection(const Vertex& p_vertex,
		                const Mat4& p_projectionMatrix) const;

		Vec3 WorldToScreen(const Vec4& p_vector) const;

		float BarycentricCoordinates(const Vertex& p_v0,
		                             const Vertex& p_v1,
		                             const Vertex& p_v2) const;

		static Mat4 CreatePerspectiveProjectionMatrix(const int& p_width,
		                                              const int& p_height,
		                                              const float& p_near,
		                                              const float& p_far,
		                                              const float& p_fov);
		static Mat4 CreatePerspectiveProjectionMatrix(const float& p_near,
		                                              const float& p_far,
		                                              const float& p_top,
		                                              const float& p_right);
		static Mat4 CreateOthographicProjection();
	};
}
