#include <iostream>
#include <iomanip>
#include <sstream>

#include "../../includes/Rendering/Window.h"

using namespace Rendering;

Window::Window(const unsigned int& p_width,
               const unsigned int& p_height) : m_width {p_width}, m_height {p_height},
                                               m_SDLManager {m_width, m_height},
                                               m_rasterizer {m_width, m_height},
                                               m_texture {m_width, m_height},
                                               m_cube {Mesh::CreateCube(Color::Red())},
                                               m_sphere {Mesh::CreateSphere(16, 16, Color::Blue())},
                                               m_triangle {
	                                               Mesh::CreateTriangle(
		                                               Vertex(Vec3(-0.5f, -0.5f, 0.f), Vec3(-0.5f, -0.5f, 0.f), Color::Red()),
		                                               Vertex(Vec3(0.5f, -0.5f, 0.f), Vec3(0.5f, -0.5f, 0.f), Color::Green()),
		                                               Vertex(Vec3(0.f, 0.5f, 0.f), Vec3(0.f, 0.5f, 0.f), Color::Blue()))
                                               },
                                               m_startClock {0.f}, m_time {0.f}, m_deltaTime {0.f}
{
//	std::cout << "Window created" << std::endl;
}

Window::~Window()
{
//	std::cout << "Window destroyed" << std::endl;
}

void Window::Setup()
{
	m_SDLManager.Setup();

	// Add Entities in Scene 0
	m_scene.AddEntity(Entity(m_cube,
	                         Mat4::CreateTranslationMatrix(Vec3(-1.f, 0.f, -2.f))));
	m_scene.AddEntity(Entity(m_sphere,
	                         Mat4::CreateTranslationMatrix(Vec3(1.f, 0, -2.f))));
	m_scene.AddEntity(Entity(m_triangle,
	                         Mat4::CreateTranslationMatrix(Vec3(0.f, 0.f, -2.f))));

	// Add Lights in Scene 0
	m_scene.AddLight(Light(Vec3(0.f, 0.f, 0.f), 0.2f, 0.4f, 0.4f));

	// Add Cameras in Scene 0
	m_scene.AddCamera(Camera(m_width, m_height, 0.1f, 10.f, 45, Vec3(0.f, 0.f, 0.f)));
}

void Window::Update()
{
	// Clear
	m_SDLManager.Clear(); // Default color is Black

	m_texture.Clear(); // Default Color is Black

	m_rasterizer.Clear();

	// Update
	m_SDLManager.Update(); // SDL CheckInput

	const_cast<Camera&>(m_scene.GetCameras()[0]).SetViewMatrix(m_SDLManager.GetView());

	// Transformation
	static float angleX = 0.f;
	static float angleY = 0.f;
	static float angleZ = 0.f;

	if (m_SDLManager.IsRotateX())
		angleX += 2.f;
	if (m_SDLManager.IsRotateY())
		angleY += 2.f;
	if (m_SDLManager.IsRotateZ())
		angleZ += 2.f;

	if (m_SDLManager.IsVersion0()) // Basic Rasterizer
	{
		// Cube Transformation
		const_cast<Entity&>(m_scene.GetEntities()[0]).SetTransform(Mat4::CreateTransformationMatrix(
			Vec3(angleX, angleY, angleZ),
			Vec3(-2.5f, 0.f, 2.f),
			Vec3(m_SDLManager.GetScale(), m_SDLManager.GetScale(), m_SDLManager.GetScale())));

		// Sphere Transformation
		const_cast<Entity&>(m_scene.GetEntities()[1]).SetTransform(Mat4::CreateTransformationMatrix(
			Vec3(angleX, angleY, angleZ),
			Vec3(2.5f, 0.f, 2.f),
			Vec3(m_SDLManager.GetScale(), m_SDLManager.GetScale(), m_SDLManager.GetScale())));

		// Triangle Transformation
		const_cast<Entity&>(m_scene.GetEntities()[2]).SetTransform(Mat4::CreateTransformationMatrix(
			Vec3(angleX, angleY, angleZ),
			Vec3(0.f, 0.f, 50.f),
			Vec3(m_SDLManager.GetScale(), m_SDLManager.GetScale(), m_SDLManager.GetScale())));

		// Triangle Color and Position
		for (size_t i = 0; i < m_triangle->GetVertices().size(); ++i)
		{
			const_cast<Vertex&>(m_triangle->GetVertices()[i]).SetColor(Color::Black());
			const_cast<Vertex&>(m_triangle->GetVertices()[i]).SetPosition(Vec3(0.f, 0.f, 0.f));
		}

		// Camera Projection Matrix
		const_cast<Camera&>(m_scene.GetCameras()[0]).SetProjectionMatrix(Rasterizer::CreateOthographicProjection());
	}
	else if (m_SDLManager.IsVersion1()) // Interpollation
	{
		// Cube Transformation
		const_cast<Entity&>(m_scene.GetEntities()[0]).SetTransform(Mat4::CreateTransformationMatrix(
			Vec3(angleX, angleY, angleZ),
			Vec3(0.f, 0.f, 50.f),
			Vec3(m_SDLManager.GetScale(), m_SDLManager.GetScale(), m_SDLManager.GetScale())));

		// Cube Color
		for (size_t i = 0; i < m_cube->GetVertices().size(); ++i)
			const_cast<Vertex&>(m_cube->GetVertices()[i]).SetColor(Color::Black());

		// Sphere Transformation
		const_cast<Entity&>(m_scene.GetEntities()[1]).SetTransform(Mat4::CreateTransformationMatrix(
			Vec3(angleX, angleY, angleZ),
			Vec3(0.5f, 0.f, 50.f),
			Vec3(m_SDLManager.GetScale(), m_SDLManager.GetScale(), m_SDLManager.GetScale())));

		// Sphere Color
		for (size_t i = 0; i < m_sphere->GetVertices().size(); ++i)
			const_cast<Vertex&>(m_sphere->GetVertices()[i]).SetColor(Color::Black());

		// Triangle
		const_cast<Entity&>(m_scene.GetEntities()[2]).SetTransform(Mat4::CreateTransformationMatrix(
			Vec3(angleX, angleY, angleZ),
			Vec3(0.f, 0.f, 2.f),
			Vec3(m_SDLManager.GetScale(), m_SDLManager.GetScale(), m_SDLManager.GetScale())));

		// Triangle Color
		const_cast<Vertex&>(m_triangle->GetVertices()[0]).SetColor(Color::Red());
		const_cast<Vertex&>(m_triangle->GetVertices()[1]).SetColor(Color::Green());
		const_cast<Vertex&>(m_triangle->GetVertices()[2]).SetColor(Color::Blue());

		// Triangle Position
		const_cast<Vertex&>(m_triangle->GetVertices()[0]).SetPosition(Vec3(-0.5f, -0.5f, 0.f));
		const_cast<Vertex&>(m_triangle->GetVertices()[1]).SetPosition(Vec3(0.5f, -0.5f, 0.f));
		const_cast<Vertex&>(m_triangle->GetVertices()[2]).SetPosition(Vec3(0.f, 0.5f, 0.f));

		// Camera Projection Matrix
		const_cast<Camera&>(m_scene.GetCameras()[0]).SetProjectionMatrix(Rasterizer::CreateOthographicProjection());
	}
	else if (m_SDLManager.IsVersion2()) // Z-Buffer
	{
		// Cube Transformation
		const_cast<Entity&>(m_scene.GetEntities()[0]).SetTransform(Mat4::CreateTransformationMatrix(
			Vec3(angleX, angleY, angleZ),
			Vec3(-0.5f, 0.f, 2.f),
			Vec3(m_SDLManager.GetScale(), m_SDLManager.GetScale(), m_SDLManager.GetScale())));

		// Sphere Transformation
		const_cast<Entity&>(m_scene.GetEntities()[1]).SetTransform(Mat4::CreateTransformationMatrix(
			Vec3(angleX, angleY, angleZ),
			Vec3(0.5f, 0.f, 2.f),
			Vec3(m_SDLManager.GetScale(), m_SDLManager.GetScale(), m_SDLManager.GetScale())));

		// Triangle Transformation
		const_cast<Entity&>(m_scene.GetEntities()[2]).SetTransform(Mat4::CreateTransformationMatrix(
			Vec3(angleX, angleY, angleZ),
			Vec3(0.f, 0.f, 50.f),
			Vec3(m_SDLManager.GetScale(), m_SDLManager.GetScale(), m_SDLManager.GetScale())));

		// Triangle Color and Position
		for (size_t i = 0; i < m_triangle->GetVertices().size(); ++i)
		{
			const_cast<Vertex&>(m_triangle->GetVertices()[i]).SetColor(Color::Black());
			const_cast<Vertex&>(m_triangle->GetVertices()[i]).SetPosition(Vec3(0.f, 0.f, 0.f));
		}

		// Camera Projection Matrix
		const_cast<Camera&>(m_scene.GetCameras()[0]).SetProjectionMatrix(Rasterizer::CreateOthographicProjection());
	}
	else if (m_SDLManager.IsVersion3()) // Phong
	{
		// Cube Transformation
		const_cast<Entity&>(m_scene.GetEntities()[0]).SetTransform(Mat4::CreateTransformationMatrix(
			Vec3(angleX, angleY, angleZ),
			Vec3(-0.5f, 0.f, 2.f),
			Vec3(m_SDLManager.GetScale(), m_SDLManager.GetScale(), m_SDLManager.GetScale())));

		// Sphere Transformation
		const_cast<Entity&>(m_scene.GetEntities()[1]).SetTransform(Mat4::CreateTransformationMatrix(
			Vec3(angleX, angleY, angleZ),
			Vec3(0.5f, 0.f, 2.f),
			Vec3(m_SDLManager.GetScale(), m_SDLManager.GetScale(), m_SDLManager.GetScale())));

		// Triangle Transformation
		const_cast<Entity&>(m_scene.GetEntities()[2]).SetTransform(Mat4::CreateTransformationMatrix(
			Vec3(angleX, angleY, angleZ),
			Vec3(0.f, 0.f, 50.f),
			Vec3(m_SDLManager.GetScale(), m_SDLManager.GetScale(), m_SDLManager.GetScale())));

		// Triangle Color and Position
		for (size_t i = 0; i < m_triangle->GetVertices().size(); ++i)
		{
			const_cast<Vertex&>(m_triangle->GetVertices()[i]).SetColor(Color::Black());
			const_cast<Vertex&>(m_triangle->GetVertices()[i]).SetPosition(Vec3(0.f, 0.f, 0.f));
		}

		// Camera Projection Matrix
		const_cast<Camera&>(m_scene.GetCameras()[0]).SetProjectionMatrix(Rasterizer::CreateOthographicProjection());
	}
	else if (m_SDLManager.IsVersion4()) // Per-Pixel & Blinn-Phong
	{
		// Cube Transformation
		const_cast<Entity&>(m_scene.GetEntities()[0]).SetTransform(Mat4::CreateTransformationMatrix(
			Vec3(angleX, angleY, angleZ),
			Vec3(-0.5f, 0.f, 2.f),
			Vec3(m_SDLManager.GetScale(), m_SDLManager.GetScale(), m_SDLManager.GetScale())));

		// Sphere Transformation
		const_cast<Entity&>(m_scene.GetEntities()[1]).SetTransform(Mat4::CreateTransformationMatrix(
			Vec3(angleX, angleY, angleZ),
			Vec3(0.5f, 0.f, 2.f),
			Vec3(m_SDLManager.GetScale(), m_SDLManager.GetScale(), m_SDLManager.GetScale())));

		// Triangle Transformation
		const_cast<Entity&>(m_scene.GetEntities()[2]).SetTransform(Mat4::CreateTransformationMatrix(
			Vec3(angleX, angleY, angleZ),
			Vec3(0.f, 0.f, 50.f),
			Vec3(m_SDLManager.GetScale(), m_SDLManager.GetScale(), m_SDLManager.GetScale())));

		// Triangle Color and Position
		for (size_t i = 0; i < m_triangle->GetVertices().size(); ++i)
		{
			const_cast<Vertex&>(m_triangle->GetVertices()[i]).SetColor(Color::Black());
			const_cast<Vertex&>(m_triangle->GetVertices()[i]).SetPosition(Vec3(0.f, 0.f, 0.f));
		}

		// Camera Projection Matrix
		const_cast<Camera&>(m_scene.GetCameras()[0]).SetProjectionMatrix(Rasterizer::CreateOthographicProjection());
	}
	else if (m_SDLManager.IsVersion5()) // Projection
	{
		// Cube Transformation
		const_cast<Entity&>(m_scene.GetEntities()[0]).SetTransform(Mat4::CreateTransformationMatrix(
			Vec3(angleX, angleY, angleZ),
			Vec3(-0.5f, 0.f, -2.f),
			Vec3(m_SDLManager.GetScale(), m_SDLManager.GetScale(), m_SDLManager.GetScale())));

		// Sphere Transformation
		const_cast<Entity&>(m_scene.GetEntities()[1]).SetTransform(Mat4::CreateTransformationMatrix(
			Vec3(angleX, angleY, angleZ),
			Vec3(0.5f, 0.f, -2.f),
			Vec3(m_SDLManager.GetScale(), m_SDLManager.GetScale(), m_SDLManager.GetScale())));

		// Triangle Transformation
		const_cast<Entity&>(m_scene.GetEntities()[2]).SetTransform(Mat4::CreateTransformationMatrix(
			Vec3(angleX, angleY, angleZ),
			Vec3(0.f, 0.f, -50.f),
			Vec3(m_SDLManager.GetScale(), m_SDLManager.GetScale(), m_SDLManager.GetScale())));

		// Triangle Color and Position
		for (size_t i = 0; i < m_triangle->GetVertices().size(); ++i)
		{
			const_cast<Vertex&>(m_triangle->GetVertices()[i]).SetColor(Color::Black());
			const_cast<Vertex&>(m_triangle->GetVertices()[i]).SetPosition(Vec3(0.f, 0.f, 0.f));
		}

		// Camera Projection Matrix
		const_cast<Camera&>(m_scene.GetCameras()[0]).SetProjectionMatrix(
			Rasterizer::CreatePerspectiveProjectionMatrix(m_width, m_height, 0.1f, 10.f, 45.f));
	}
	else if (m_SDLManager.IsVersion6()) // Texture
	{
		// TODO Do Texture
	}
	else if (m_SDLManager.IsVersion7()) // Wireframe
	{
		// Cube Transformation
		const_cast<Entity&>(m_scene.GetEntities()[0]).SetTransform(Mat4::CreateTransformationMatrix(
			Vec3(angleX, angleY, angleZ),
			Vec3(-0.5f, 0.f, -2.f),
			Vec3(m_SDLManager.GetScale(), m_SDLManager.GetScale(), m_SDLManager.GetScale())));

		// Sphere Transformation
		const_cast<Entity&>(m_scene.GetEntities()[1]).SetTransform(Mat4::CreateTransformationMatrix(
			Vec3(angleX, angleY, angleZ),
			Vec3(0.5f, 0.f, -2.f),
			Vec3(m_SDLManager.GetScale(), m_SDLManager.GetScale(), m_SDLManager.GetScale())));

		// Triangle Transformation
		const_cast<Entity&>(m_scene.GetEntities()[2]).SetTransform(Mat4::CreateTransformationMatrix(
			Vec3(angleX, angleY, angleZ),
			Vec3(0.f, 0.f, -50.f),
			Vec3(m_SDLManager.GetScale(), m_SDLManager.GetScale(), m_SDLManager.GetScale())));

		// Triangle Color and Position
		for (size_t i = 0; i < m_triangle->GetVertices().size(); ++i)
		{
			const_cast<Vertex&>(m_triangle->GetVertices()[i]).SetColor(Color::Black());
			const_cast<Vertex&>(m_triangle->GetVertices()[i]).SetPosition(Vec3(0.f, 0.f, 0.f));
		}

		// Camera Projection Matrix
		const_cast<Camera&>(m_scene.GetCameras()[0]).SetProjectionMatrix(
			Rasterizer::CreatePerspectiveProjectionMatrix(m_width, m_height, 0.1f, 10.f, 45.f));
	}
	else if (m_SDLManager.IsVersion8()) // Alpha-Blending & Back-Face Culling
	{
		// TODO Do Alpha-Blending & Back-Face Culling
	}
	else if (m_SDLManager.IsVersion9()) // Camera
	{
		// Cube Transformation
		const_cast<Entity&>(m_scene.GetEntities()[0]).SetTransform(Mat4::CreateTransformationMatrix(
			Vec3(angleX, angleY, angleZ),
			Vec3(-1.f, 0.f, -2.f),
			Vec3(m_SDLManager.GetScale(), m_SDLManager.GetScale(), m_SDLManager.GetScale())));

		// Sphere Transformation
		const_cast<Entity&>(m_scene.GetEntities()[1]).SetTransform(Mat4::CreateTransformationMatrix(
			Vec3(angleX, angleY, angleZ),
			Vec3(1.f, 0.f, -2.f),
			Vec3(m_SDLManager.GetScale(), m_SDLManager.GetScale(), m_SDLManager.GetScale())));

		// Triangle Transformation
		const_cast<Entity&>(m_scene.GetEntities()[2]).SetTransform(Mat4::CreateTransformationMatrix(
			Vec3(angleX, angleY, angleZ),
			Vec3(0.f, 0.f, -50.f),
			Vec3(m_SDLManager.GetScale(), m_SDLManager.GetScale(), m_SDLManager.GetScale())));

		// Triangle Color and Position
		for (size_t i = 0; i < m_triangle->GetVertices().size(); ++i)
		{
			const_cast<Vertex&>(m_triangle->GetVertices()[i]).SetColor(Color::Black());
			const_cast<Vertex&>(m_triangle->GetVertices()[i]).SetPosition(Vec3(0.f, 0.f, 0.f));
		}

		// Camera Projection Matrix
		const_cast<Camera&>(m_scene.GetCameras()[0]).SetProjectionMatrix(
			Rasterizer::CreatePerspectiveProjectionMatrix(m_width, m_height, 0.1f, 10.f, 45.f));
	}
	else if (m_SDLManager.IsVersion10()) // Anti-Aliasing
	{
		// TODO Do Anti-Aliasing
	}
	else
	{
		// Cube Transformation
		const_cast<Entity&>(m_scene.GetEntities()[0]).SetTransform(Mat4::CreateTransformationMatrix(
			Vec3(angleX, angleY, angleZ),
			Vec3(-1.f, 0.f, -2.f),
			Vec3(m_SDLManager.GetScale(), m_SDLManager.GetScale(), m_SDLManager.GetScale())));

		// Cube Color
		for (size_t i = 0; i < m_cube->GetVertices().size(); ++i)
			const_cast<Vertex&>(m_cube->GetVertices()[i]).SetColor(Color::Red());

		// Sphere Transformation
		const_cast<Entity&>(m_scene.GetEntities()[1]).SetTransform(Mat4::CreateTransformationMatrix(
			Vec3(angleX, angleY, angleZ),
			Vec3(1.f, 0.f, -2.f),
			Vec3(m_SDLManager.GetScale(), m_SDLManager.GetScale(), m_SDLManager.GetScale())));

		// Sphere Color
		for (size_t i = 0; i < m_sphere->GetVertices().size(); ++i)
			const_cast<Vertex&>(m_sphere->GetVertices()[i]).SetColor(Color::Blue());

		// Triangle Transformation
		const_cast<Entity&>(m_scene.GetEntities()[2]).SetTransform(Mat4::CreateTransformationMatrix(
			Vec3(angleX, angleY, angleZ),
			Vec3(0.f, 0.f, -2.f),
			Vec3(m_SDLManager.GetScale(), m_SDLManager.GetScale(), m_SDLManager.GetScale())));

		// Triangle Color
		const_cast<Vertex&>(m_triangle->GetVertices()[0]).SetColor(Color::Red());
		const_cast<Vertex&>(m_triangle->GetVertices()[1]).SetColor(Color::Green());
		const_cast<Vertex&>(m_triangle->GetVertices()[2]).SetColor(Color::Blue());

		// Triangle Position
		const_cast<Vertex&>(m_triangle->GetVertices()[0]).SetPosition(Vec3(-0.5f, -0.5f, 0.f));
		const_cast<Vertex&>(m_triangle->GetVertices()[1]).SetPosition(Vec3(0.5f, -0.5f, 0.f));
		const_cast<Vertex&>(m_triangle->GetVertices()[2]).SetPosition(Vec3(0.f, 0.5f, 0.f));

		// Camera Projection Matrix
		const_cast<Camera&>(m_scene.GetCameras()[0]).SetProjectionMatrix(
			Rasterizer::CreatePerspectiveProjectionMatrix(m_width, m_height, 0.1f, 10.f, 45.f));
	}

	// World Transformation
	for (size_t i = 0; i < m_scene.GetEntities().size(); ++i)
		const_cast<Entity&>(m_scene.GetEntities()[i]).SetTransform(Mat4::CreateTransformationMatrix(
			Vec3(angleX, angleY, angleZ),
			Vec3(m_scene.GetEntities()[i].GetTransform().data[0][3], m_scene.GetEntities()[i].GetTransform().data[1][3],
			     m_scene.GetEntities()[i].GetTransform().data[2][3]),
			Vec3(m_SDLManager.GetScale(), m_SDLManager.GetScale(), m_SDLManager.GetScale())));

	// Rasterizer (Magic happens here)
	m_rasterizer.RenderScene(&m_scene,
	                         &m_texture,
	                         &m_SDLManager);
}

void Window::Display()
{
	// Draw Texture
	m_texture.DrawPixels(m_SDLManager);

	// Draw Texts
	std::ostringstream fps;
	fps << std::setprecision(3) << m_deltaTime;
	m_SDLManager.DrawText("FPS: " + fps.str(), 70, 10, 40, 45);

	m_SDLManager.DrawText("Light: ", 70, 30, 40, 45);
	if (m_SDLManager.IsLight())
	{
		if (m_SDLManager.IsPhong())
			m_SDLManager.DrawText("Phong", 190, 30, 40, 45);
		else
			m_SDLManager.DrawText("Blinn-Phong", 190, 30, 40, 45);
	}
	else
		m_SDLManager.DrawText("OFF", 190, 30, 40, 45);

	m_SDLManager.DrawText("Wireframe: ", 70, 50, 40, 45);
	if (m_SDLManager.IsWireframe())
		m_SDLManager.DrawText("ON", 330, 50, 40, 45);
	else
		m_SDLManager.DrawText("OFF", 330, 50, 40, 45);

	m_SDLManager.DrawText("Normal: ", 70, 70, 40, 45);
	if (m_SDLManager.IsNormal())
		m_SDLManager.DrawText("ON", 240, 70, 40, 45);
	else
		m_SDLManager.DrawText("OFF", 240, 70, 40, 45);

	// SDL RenderPresent
	m_SDLManager.Display();
}

void Window::Run()
{
	while (m_SDLManager.IsRunning())
	{
		m_startClock = static_cast<float>(SDL_GetTicks());

		Update();

		Display();

		m_deltaTime = 1000.f / (SDL_GetTicks() - m_startClock);
		m_time += m_deltaTime;
	}
}

unsigned Window::GetWidth() const { return m_width; }
unsigned Window::GetHeight() const { return m_height; }
