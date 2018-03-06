#include <iostream>
#include <cmath>

#include "../../includes/Maths/Vec4.h"

using namespace Maths;

Vec4::Vec4(const float& p_x,
           const float& p_y,
           const float& p_z,
           const float& p_w) : x {p_x}, y {p_y}, z {p_z}, w {p_w}
{
	//	std::cout << "Vec4 created" << std::endl;
}

Vec4::Vec4(const Vec3& p_vector,
           const float& p_w) : x {p_vector.x},
                               y {p_vector.y},
                               z {p_vector.z},
                               w {p_w}
{
//	std::cout << "Vec4 created" << std::endl;
}

Vec4::~Vec4()
{
//	std::cout << "Vec4 detroyed" << std::endl;
}

float Vec4::GetMagnitude() const
{
	if (w != 1.f) //If not homogenized
		return sqrt(x / w * x / w + y / w * y / w + z / w * z / w);
	return sqrt(x * x + y * y + z * z);
}

Vec4 Vec4::Normalize() const
{
	Vec4 normalize(x, y, z, w);

	const float magnitude = GetMagnitude();

	if (magnitude != 0)
	{
		normalize.x /= magnitude;
		normalize.y /= magnitude;
		normalize.z /= magnitude;
	}

	return normalize;
}

Vec4 Vec4::Homogenize() const
{
	Vec4 homogenize(x, y, z, w);

	if (w != 0 && w != 1)
	{
		homogenize.x /= w;
		homogenize.y /= w;
		homogenize.z /= w;
		homogenize.w = 1.f;
	}

	return homogenize;
}

Vec4 Vec4::operator+(const Vec4& p_vector) const
{
	Vec4 add(0, 0, 0);

	add.x = x + p_vector.x;
	add.y = y + p_vector.y;
	add.z = z + p_vector.z;

	return add;
}

Vec4 Vec4::operator*(const float& p_scalar) const
{
	Vec4 mult(0, 0, 0);

	mult.x = x * p_scalar;
	mult.y = y * p_scalar;
	mult.z = z * p_scalar;

	return mult;
}

Vec3 Vec4::ToVec3() const { return Vec3(Homogenize().x, Homogenize().y, Homogenize().z); }
