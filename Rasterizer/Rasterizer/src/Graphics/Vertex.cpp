#include <iostream>

#include "../../includes/Graphics/Vertex.h"

using namespace Graphics;

Vertex::Vertex(const Vec3& p_position,
               const Vec3& p_normal,
               const Color& p_color) : m_position {p_position},
                                       m_normal {p_normal.Normalize()},
                                       m_color {p_color}
{
//	std::cout << "Vertex created" << std::endl;
}

Vertex::~Vertex()
{
//	std::cout << "Vertex destroyed" << std::endl;
}

void Vertex::SetPosition(const Vec3& p_position) { m_position = p_position; }
void Vertex::SetNormal(const Vec3& p_normal) { m_normal = p_normal; }
void Vertex::SetColor(const Color& p_color) { m_color = p_color; }

const Vec3& Vertex::GetPosition() const { return m_position; }
const Vec3& Vertex::GetNormal() const { return m_normal; }

const Color& Vertex::GetColor() const { return m_color; }
