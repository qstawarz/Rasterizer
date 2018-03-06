#include <iostream>
#include <algorithm>

#include "../../includes/Graphics/Color.h"
#include "../../includes/Rendering/Rasterizer.h"

using namespace Rendering;
using namespace Graphics;

Rasterizer::Rasterizer(const unsigned int& p_width,
                       const unsigned int& p_height) : m_width {p_width}, m_height {p_height},
                                                       m_ZBuffer {new float[m_width * m_height]}
{
//	std::cout << "Rasterizer created" << std::endl;
}

Rasterizer::~Rasterizer()
{
	delete[] m_ZBuffer;

//	std::cout << "Rasterizer destroyed" << std::endl;
}

void Rasterizer::RenderScene(const Scene* p_scene, const Texture* p_target, const SDLManager* p_SDLManager) const
{
	const std::vector<Entity>& entities = p_scene->GetEntities();
	const std::vector<Light>& lights = p_scene->GetLights();

	//	p_target->Clear(); // TODO Why here ? #instructions

	for (size_t i = 0; i < entities.size(); ++i)
	{
		const std::vector<Vertex>& vertices = entities[i].GetMesh()->GetVertices();
		const std::vector<int>& indices = entities[i].GetMesh()->GetIndices();
		const Mat4& transformMatrix = entities[i].GetTransform();

		for (size_t j = 0; j < indices.size(); j += 3)
		{
			// Find Indices
			const size_t& a = indices[j];
			const size_t& b = indices[j + 1];
			const size_t& c = indices[j + 2];

			// Vertices
			Vertex v0 = vertices[a];
			Vertex v1 = vertices[b];
			Vertex v2 = vertices[c];

			// Apply tranform for each vertex
			v0 = Transform(v0, p_scene->GetCameras()[0].GetViewMatrix(), transformMatrix);
			v1 = Transform(v1, p_scene->GetCameras()[0].GetViewMatrix(), transformMatrix);
			v2 = Transform(v2, p_scene->GetCameras()[0].GetViewMatrix(), transformMatrix);

			// Apply light for each vertex
			if (p_SDLManager->IsLight())
			{
				if (p_SDLManager->IsPhong())
				{
					// Apply Phong Light for each vertex
					v0 = PhongMethod(v0, lights, p_scene->GetCameras()[0].GetViewMatrix());
					v1 = PhongMethod(v1, lights, p_scene->GetCameras()[0].GetViewMatrix());
					v2 = PhongMethod(v2, lights, p_scene->GetCameras()[0].GetViewMatrix());
				}
				else
				{
					// Apply Blinn-Phong Light for each vertex
					v0 = BlinnPhongMethod(v0, lights, p_scene->GetCameras()[0].GetViewMatrix());
					v1 = BlinnPhongMethod(v1, lights, p_scene->GetCameras()[0].GetViewMatrix());
					v2 = BlinnPhongMethod(v2, lights, p_scene->GetCameras()[0].GetViewMatrix());
				}
			}

			// Draw Normal
			if (p_SDLManager->IsNormal())
				Normal(v0, v1, v2, p_scene->GetCameras()[0].GetProjectionMatrix(), p_target);

			// Apply Projection for each vertex
			std::vector<Vec4> vectors;
			vectors.emplace_back(Projection(v0, p_scene->GetCameras()[0].GetProjectionMatrix()));
			vectors.emplace_back(Projection(v1, p_scene->GetCameras()[0].GetProjectionMatrix()));
			vectors.emplace_back(Projection(v2, p_scene->GetCameras()[0].GetProjectionMatrix()));

			// Apply Clipping for each vertex
			Clipping(vectors);

			// Apply WorldToScreen coordinates for each vertex
			v0.SetPosition(WorldToScreen(vectors[0]));
			v1.SetPosition(WorldToScreen(vectors[1]));
			v2.SetPosition(WorldToScreen(vectors[2]));

			// Draw Triangle
			// Choose between Bresenham or Barycentric method
			if (p_SDLManager->IsWireframe())
				BresenhamMethod(v0, v1, v2, p_target); // TODO Review Wireframe (Apply Z-Buffer)
			else
				BarycentricMethod(v0, v1, v2, p_target);
		}
	}
}

void Rasterizer::BresenhamCoordinates(const Vertex& p_v0, const Vertex& p_v1, const Texture* p_target) const
{
	const float& x1 = p_v0.GetPosition().x;
	const float& y1 = p_v0.GetPosition().y;
	const float& x2 = p_v1.GetPosition().x;
	const float& y2 = p_v1.GetPosition().y;

	float x = x1;
	float y = y1;

	bool changed = false;

	// Get estimation of length
	float dx = abs(x2 - x1);
	float dy = abs(y2 - y1);

	// Get direction of line
	float signX = 0;
	if (x2 - x1 > 0) signX = 1.0f;
	if (x2 - x1 < 0) signX = -1.0f;

	float signY = 0;
	if (y2 - y1 > 0) signY = 1.0f;
	if (y2 - y1 < 0) signY = -1.0f;

	if (dy > dx)
	{
		//Swap values
		std::swap(dx, dy);
		changed = true;
	}

	// Calculate starting error value
	float e = dy / dx - 0.5f;

	for (int i = 0; i <= dx; ++i)
	{
		p_target->SetPixelColor(static_cast<int>(x),
		                        static_cast<int>(y),
		                        p_v0.GetColor());

		while (e >= 0)
		{
			if (changed)
				x += signX;
			else
				y += signY;
			e -= 1;
		}

		if (changed)
			y += signY;
		else
			x += signX;

		e += dy / dx;
	}

	// Draw final point
	p_target->SetPixelColor(static_cast<int>(x2),
	                        static_cast<int>(y2),
	                        p_v1.GetColor());
}

void Rasterizer::BresenhamMethod(const Vertex& p_v0, const Vertex& p_v1, const Vertex& p_v2,
                                 const Texture* p_target) const
{
	BresenhamCoordinates(p_v1, p_v2, p_target);
	BresenhamCoordinates(p_v2, p_v0, p_target); // Not necessary for Cube (Draw the diagonal for each face)
	BresenhamCoordinates(p_v0, p_v1, p_target);
}

void Rasterizer::BarycentricMethod(const Vertex& p_v0, const Vertex& p_v1, const Vertex& p_v2,
                                   const Texture* p_target) const
{
	const float area = BarycentricCoordinates(p_v0, p_v1, p_v2);

	const int& x0 = static_cast<int>(p_v0.GetPosition().x);
	const int& y0 = static_cast<int>(p_v0.GetPosition().y);
	const int& x1 = static_cast<int>(p_v1.GetPosition().x);
	const int& y1 = static_cast<int>(p_v1.GetPosition().y);
	const int& x2 = static_cast<int>(p_v2.GetPosition().x);
	const int& y2 = static_cast<int>(p_v2.GetPosition().y);

	// Bounding box of the triangle
	unsigned int xMin = std::min(std::min(x0, x1), x2);
	unsigned int yMin = std::min(std::min(y0, y1), y2);
	unsigned int xMax = std::max(std::max(x0, x1), x2);
	unsigned int yMax = std::max(std::max(y0, y1), y2);

	// Clip the texture
	xMin = (xMin < 0u) ? 0u : xMin;
	yMin = (yMin < 0u) ? 0u : yMin;
	xMax = (xMax > m_width) ? m_width : xMax;
	yMax = (yMax > m_height) ? m_height : yMax;

	// SetColor for pixel in the triangle
	for (unsigned int x = xMin; x <= xMax; ++x)
		for (unsigned int y = yMin; y <= yMax; ++y)
		{
			const Vertex vertex(Vec3(static_cast<float>(x), static_cast<float>(y), 0));

			float w0 = BarycentricCoordinates(p_v1, p_v2, vertex);
			float w1 = BarycentricCoordinates(p_v2, p_v0, vertex);
			float w2 = BarycentricCoordinates(p_v0, p_v1, vertex);

			// Inside Triangle
			if (w0 >= 0 && w1 >= 0 && w2 >= 0)
			{
				w0 /= area;
				w1 /= area;
				w2 /= area;

				const float z = 1 / (w0 / p_v0.GetPosition().z + w1 / p_v1.GetPosition().z + w2 / p_v2.GetPosition().z);

				// TODO Review Z-Buffer condition (Check with projection matrix)
				// Z-Buffer
				if (z < m_ZBuffer[y * m_width + x])
				{
					// TODO Do Per-pixel lighting

					p_target->SetPixelColor(x, y, Color(
						                        static_cast<unsigned char>(w0 * p_v0.GetColor().r + w1 * p_v1.GetColor().r + w2 * p_v2.GetColor().r),
						                        static_cast<unsigned char>(w0 * p_v0.GetColor().g + w1 * p_v1.GetColor().g + w2 * p_v2.GetColor() .g),
						                        static_cast<unsigned char>(w0 * p_v0.GetColor().b + w1 * p_v1.GetColor().b + w2 * p_v2.GetColor().b),
						                        static_cast<unsigned char>(w0 * p_v0.GetColor().a + w1 * p_v1.GetColor().a + w2 * p_v2.GetColor().a)));

					m_ZBuffer[y * m_width + x] = z;
				}
			}
		}
}

void Rasterizer::Normal(const Vertex& p_v0, const Vertex& p_v1, const Vertex& p_v2, const Mat4& p_projectionMatrix,
                        const Texture* p_target) const
{
	// Normals
	Vertex n0 = Vertex(p_v0.GetNormal() * 0.5f + p_v0.GetPosition());
	Vertex n1 = Vertex(p_v1.GetNormal() * 0.5f + p_v1.GetPosition());
	Vertex n2 = Vertex(p_v2.GetNormal() * 0.5f + p_v2.GetPosition());

	n0.SetPosition(WorldToScreen(Projection(n0, p_projectionMatrix)));
	n1.SetPosition(WorldToScreen(Projection(n1, p_projectionMatrix)));
	n2.SetPosition(WorldToScreen(Projection(n2, p_projectionMatrix)));

	// Positions
	Vertex p0 = Vertex(p_v0.GetPosition());
	Vertex p1 = Vertex(p_v1.GetPosition());
	Vertex p2 = Vertex(p_v2.GetPosition());

	p0.SetPosition(WorldToScreen(Projection(p0, p_projectionMatrix)));
	p1.SetPosition(WorldToScreen(Projection(p1, p_projectionMatrix)));
	p2.SetPosition(WorldToScreen(Projection(p2, p_projectionMatrix)));

	BresenhamCoordinates(p0, n0, p_target);
	BresenhamCoordinates(p1, n1, p_target);
	BresenhamCoordinates(p2, n2, p_target);
}

void Rasterizer::Clipping(std::vector<Vec4>& p_vectors) const
{
	// TODO Do Clipping of pespective
}

void Rasterizer::Clear() const
{
	for (unsigned int i = 0; i < m_width * m_height; ++i)
		m_ZBuffer[i] = FLT_MAX;
}

Vertex Rasterizer::Transform(const Vertex& p_vertex, const Mat4& p_inverseCameraMatrix,
                             const Mat4& p_transformMatrix) const
{
	Vec4 position(p_vertex.GetPosition());
	Vec4 normal(p_vertex.GetNormal());

	position = p_inverseCameraMatrix * p_transformMatrix * position;
	normal = (p_transformMatrix.GetInverse().GetTranspose() * normal).Normalize();

	return Vertex(position.ToVec3(), Vec3(normal.x, normal.y, normal.z), p_vertex.GetColor());
}

Vertex Rasterizer::PhongMethod(const Vertex& p_vertex, const std::vector<Light>& p_lights,
                               const Mat4& p_inverseCameraMatrix) const
{
	Color color;

	for (size_t i = 0; i < p_lights.size(); ++i)
	{
		const float shininess = 1.f;

		const Vec3& lightDir = ((p_inverseCameraMatrix * Vec4(p_lights[i].GetPosition())).ToVec3() - p_vertex.GetPosition()).Normalize();
		const Vec3& surfaceNormal = p_vertex.GetNormal().Normalize();
		const Vec3& viewDir = ((p_inverseCameraMatrix.GetInverse() * Vec4(0.f, 0.f, 0.f)).ToVec3() - p_vertex.GetPosition()).Normalize();
		const Vec3& reflectionDir = (surfaceNormal * (lightDir * surfaceNormal) * 2.f - lightDir).Normalize();

		const float ambiant = p_lights[i].GetAmbiantComponent();
		const float diffuse = p_lights[i].GetDiffuseComponent() * std::clamp(surfaceNormal * lightDir, 0.f, 1.f);
		const float specular = (surfaceNormal * lightDir > 0.f) ? p_lights[i].GetSpecularComponent() * powf(std::clamp((viewDir * reflectionDir), 0.f, 1.f), shininess) : 0.f;

		color.r += static_cast<unsigned char>(p_vertex.GetColor().r * (ambiant + diffuse + specular));
		color.g += static_cast<unsigned char>(p_vertex.GetColor().g * (ambiant + diffuse + specular));
		color.b += static_cast<unsigned char>(p_vertex.GetColor().b * (ambiant + diffuse + specular));
		color.a += static_cast<unsigned char>(p_vertex.GetColor().a * (ambiant + diffuse + specular));
	}

	return Vertex(p_vertex.GetPosition(), p_vertex.GetNormal(), color);
}

Vertex Rasterizer::BlinnPhongMethod(const Vertex& p_vertex, const std::vector<Light>& p_lights,
                                    const Mat4& p_inverseCameraMatrix) const
{
	Color color;

	for (size_t i = 0; i < p_lights.size(); ++i)
	{
		const float shininess = 1.f;

		const Vec3& lightDir = ((p_inverseCameraMatrix * Vec4(p_lights[i].GetPosition())).ToVec3() - p_vertex.GetPosition()).
			Normalize();
		const Vec3& surfaceNormal = (p_vertex.GetNormal()).Normalize();
		const Vec3& viewDir = ((p_inverseCameraMatrix.GetInverse() * Vec4(0.f, 0.f, 0.f)).ToVec3() - p_vertex.GetPosition()).
			Normalize();
		const Vec3& halfwayDir = (lightDir + viewDir).Normalize();

		const float ambiant = p_lights[i].GetAmbiantComponent();
		const float diffuse = p_lights[i].GetDiffuseComponent() * std::clamp(surfaceNormal * lightDir, 0.f, 1.f);
		const float specular = (surfaceNormal * lightDir > 0.f) ? p_lights[i].GetSpecularComponent() * powf(std::clamp((surfaceNormal * halfwayDir), 0.f, 1.f), shininess) : 0.f;

		color.r += static_cast<unsigned char>(p_vertex.GetColor().r * (ambiant + diffuse + specular));
		color.g += static_cast<unsigned char>(p_vertex.GetColor().g * (ambiant + diffuse + specular));
		color.b += static_cast<unsigned char>(p_vertex.GetColor().b * (ambiant + diffuse + specular));
		color.a += static_cast<unsigned char>(p_vertex.GetColor().a * (ambiant + diffuse + specular));
	}

	return Vertex(p_vertex.GetPosition(), p_vertex.GetNormal(), color);
}

Vec4 Rasterizer::Projection(const Vertex& p_vertex, const Mat4& p_projectionMatrix) const
{
	return Vec4(p_projectionMatrix * Vec4(p_vertex.GetPosition()));
}

Vec3 Rasterizer::WorldToScreen(const Vec4& p_vector) const
{
	const Vec4& position = p_vector.Homogenize();

	float x = position.x;
	float y = position.y;
	const float& z = position.z;

	x = ((x / 5.f) + 1) * 0.5f * m_width;
	y = m_height - ((y / 5.f) + 1) * 0.5f * m_height;

	return Vec3(x, y, z);
}

float Rasterizer::BarycentricCoordinates(const Vertex& p_v0, const Vertex& p_v1, const Vertex& p_v2) const
{
	return ((p_v2.GetPosition().x - p_v1.GetPosition().x) * (p_v1.GetPosition().y - p_v0.GetPosition().y) -
			(p_v2.GetPosition().y - p_v1.GetPosition().y) * (p_v1.GetPosition().x - p_v0.GetPosition().x));
}

Mat4 Rasterizer::CreatePerspectiveProjectionMatrix(const int& p_width, const int& p_height, const float& p_near,
                                                   const float& p_far, const float& p_fov)
{
	Mat4 projection;

	const float& ratio = static_cast<float>(p_width / static_cast<float>(p_height));
	const float scale = 1.f / static_cast<float>(tan(p_fov * 0.5f * (M_PI / 180.f)));

	projection.data[0][0] = scale;
	projection.data[1][1] = scale * ratio;
	projection.data[2][2] = -p_far / (p_far - p_near);
	projection.data[2][3] = -(p_far * p_near) / (p_far - p_near);
	projection.data[3][2] = -1.f;

	return projection;
}

Mat4 Rasterizer::CreatePerspectiveProjectionMatrix(const float& p_near, const float& p_far, const float& p_top,
                                                   const float& p_right)
{
	Mat4 projection;

	projection.data[0][0] = p_near / p_right;
	projection.data[1][1] = p_near / p_top;
	projection.data[2][2] = -(p_far + p_near) / (p_far - p_near);
	projection.data[2][3] = - 2.f * (p_far * p_near) / (p_far - p_near);
	projection.data[3][2] = -1.f;

	return projection;
}

Mat4 Rasterizer::CreateOthographicProjection()
{
	Mat4 projection;

	projection.data[0][0] = 1.f;
	projection.data[1][1] = 1.f;
	projection.data[2][2] = 1.f;
	projection.data[3][3] = 1.f;

	return projection;
}
