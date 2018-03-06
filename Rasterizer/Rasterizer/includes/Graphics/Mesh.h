#pragma once

#include <vector>

#include "Vertex.h"

namespace Graphics
{
	class Mesh
	{
	private:
		std::vector<Vertex> m_vertices;

		std::vector<int> m_indices;

	public:
		Mesh();
		~Mesh();

		void AddTriangleIndices(const int& p_a,
		                        const int& p_b,
		                        const int& p_c);

		static Mesh* CreateCube(const Color& p_color = Color::White());
		static Mesh* CreateSphere(const int& p_latitudeCount,
		                          const int& p_longitudeCount,
		                          const Color& p_color = Color::White());
		static Mesh* CreateTriangle(const Vertex& p_a,
		                            const Vertex& p_b,
		                            const Vertex& p_c);

		const std::vector<Vertex>& GetVertices() const;
		const std::vector<int>& GetIndices() const;
	};
}
